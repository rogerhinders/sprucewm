#include "statusbar.h"

static struct window *sb_wnd = NULL;
static struct linked_list *l_widgets = NULL;
static pthread_t t_handle;
static bool t_running = false;
static struct button *dock_btn = NULL;

static void dock_clickhandler(bool pressed, void *arg) {
	if(pressed)
		wm_show_docked();
	else
		wm_hide_docked();
}

static void *update_thread(void *arg) {
	struct widget *wdg;

	while(t_running) {
		/* idle */
		sleep(STATUSBAR_UPDATE_INTERVAL);

		/* update widgets */
		if(l_widgets == NULL)
			continue;

		linked_list_rewind(l_widgets);

		while((wdg = linked_list_next(l_widgets)) != NULL)
			widget_update(wdg);

		statusbar_update();
	}

	return NULL;
}

bool statusbar_init() {
	struct window *r_wnd = xserver_get_root_wnd();
	uint32_t v_mask;
	uint32_t v_list[2];

	if(r_wnd == NULL) {
		return false;
	}

	/* init window */
	sb_wnd = window_create(xcb_generate_id(xserver_get_conn()));

	window_setsize(sb_wnd, r_wnd->w, STATUSBAR_HEIGHT);
	window_setcoords(sb_wnd, 0, r_wnd->h - sb_wnd->h);

	v_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	v_list[0] = xserver_screen_get_white();
	v_list[1] = XCB_EVENT_MASK_BUTTON_PRESS;

	xcb_create_window(
			xserver_get_conn(),
			xserver_screen_get_depth(),
			sb_wnd->handle,
			r_wnd->handle,
			sb_wnd->x, /* pos X */
			sb_wnd->y, /* pos Y */
			sb_wnd->w, /* width */
			sb_wnd->h, /* height */
			1, /* border width */
			XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class */
			xserver_screen_get_visual(),
			v_mask, /* v-mask */
			v_list); /* v-list */

	xserver_map_window(sb_wnd);

	xserver_flush_conn();

	/* init widget related stuff */
	l_widgets = linked_list_create();

	/* init dock toggle btn */
	dock_btn = button_create(sb_wnd);
	button_setsize(dock_btn, STATUSBAR_DOCK_BUTTON_WIDTH, STATUSBAR_HEIGHT);
	button_settext(dock_btn, "KBD");
	button_setcoords(dock_btn, STATUSBAR_PADDING_LEFT, 0);
	button_set_text_padding(dock_btn, 23, 33, 0, 0);
	button_setpressed(dock_btn, true);
	button_setclick_handler(dock_btn, dock_clickhandler);

	return true;
}

void statusbar_cleanup() {
	button_destroy(dock_btn);
	window_destroy(sb_wnd);
	linked_list_destroy(l_widgets);
}

void statusbar_update() {
	struct widget *wdg;

	xcb_gcontext_t fgc = xserver_create_font_gc(
			xserver_screen_get_white(),
			xserver_screen_get_black(),
			"fixed",
			sb_wnd);

	uint32_t n_widgets = linked_list_count(l_widgets);

	if(n_widgets == 0)
		return;

	uint32_t wdg_w = (
			statusbar_get_width() - STATUSBAR_PADDING_LEFT * 2
			- STATUSBAR_DOCK_BUTTON_WIDTH) / n_widgets;
	uint32_t x = STATUSBAR_PADDING_LEFT * 2 + STATUSBAR_DOCK_BUTTON_WIDTH;
	uint32_t y = 20;

	linked_list_rewind(l_widgets);

	while((wdg = linked_list_next(l_widgets)) != NULL) {
		xcb_image_text_8(
				xserver_get_conn(),
				strlen(wdg->text),
				sb_wnd->handle,
				fgc,
				x, y,
				wdg->text);

		x += wdg_w;
	}

	/* draw dock btn */
	button_draw(dock_btn);

	xcb_free_gc(xserver_get_conn(), fgc);
	xserver_flush_conn();
}

struct window *statusbar_get_window() {
	return sb_wnd;
}

uint32_t statusbar_get_width() {
	if(sb_wnd == NULL)
		return 0;

	return sb_wnd->w;
}

uint32_t statusbar_get_height() {
	if(sb_wnd == NULL)
		return 0;

	return sb_wnd->h;
}

void statusbar_onclick(uint32_t x, uint32_t y) {
	if(button_coord_in_bounds(dock_btn, x, y))
		button_onclick(dock_btn, NULL);
}

void statusbar_register_widget(struct widget *wdg) {
	if(l_widgets == NULL)
		return;

	linked_list_add(l_widgets, wdg);
}

void statusbar_unregister_widget(struct widget *wdg) {
	if(l_widgets == NULL)
		return;

	linked_list_remove(l_widgets, wdg);
}

bool statusbar_start_update_thread() {
	if(pthread_create(&t_handle, NULL, update_thread, NULL) != 0)
		return false;

	t_running = true;
	return true;
}

void statusbar_stop_update_thread() {
	t_running = false;
}
