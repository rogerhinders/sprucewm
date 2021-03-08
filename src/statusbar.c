#include "statusbar.h"

static struct window *sb_wnd = NULL;
static xcb_gcontext_t bg_gc_click;
static xcb_gcontext_t bg_gc_noclick;
static struct linked_list *l_widgets = NULL;

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

	/* init drawables */
	bg_gc_click = xserver_create_drawable_gc(
			xserver_screen_get_white(), xserver_screen_get_black(), sb_wnd);

	bg_gc_noclick = xserver_create_drawable_gc(
			xserver_screen_get_white(), xserver_screen_get_white(), sb_wnd);

	/* init widget related stuff */
	l_widgets = linked_list_create();

	return true;
}

void statusbar_cleanup() {
	xcb_free_gc(xserver_get_conn(), bg_gc_click);
	xcb_free_gc(xserver_get_conn(), bg_gc_noclick);
	window_destroy(sb_wnd);
	linked_list_destroy(l_widgets);
}

void statusbar_update() {
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
}
