#include "taskbar.h"

static struct window *tb_wnd = NULL;
static xcb_gcontext_t bg_gc_click;
static xcb_gcontext_t bg_gc_noclick;

static void button_click_handler(bool pressed, void *arg) {
	struct window *wnd = (struct window*)arg;

	if(wnd == NULL) {
		printf("bad click arg\n");
		return;
	}

	printf("toggling window %x\n", wnd->handle);

	if(pressed)
		xserver_map_window(wnd);
	else
		xserver_unmap_window(wnd);
}

static void setup_window_btn(
		struct window *wnd,
		uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
	struct button *btn = button_create();
	window_set_toggle_button(wnd, btn);
	button_setcoords(btn, x, y);
	button_setsize(btn, w, h);
	button_settext(btn, wnd->name);
	button_setpressed(btn, true);
	button_setclick_handler(btn, button_click_handler);
}

static void draw_button(
		struct window *wnd,
		xcb_gcontext_t font_gc_noclick,
		xcb_gcontext_t font_gc_click) {
	if(wnd->toggle_btn == NULL) {
		return;
	}

	xcb_gcontext_t gc = wnd->toggle_btn->pressed ? 
			font_gc_click : font_gc_noclick;

	xcb_gcontext_t bg_gc = wnd->toggle_btn->pressed ?
		bg_gc_click : bg_gc_noclick;

	/* draw background */
	xcb_rectangle_t rect = {
		wnd->toggle_btn->x, 0,
		wnd->toggle_btn->w, wnd->toggle_btn->h
	};

	xcb_poly_fill_rectangle(
			xserver_get_conn(), tb_wnd->handle, bg_gc, 1, &rect);

	/* draw text */
	xcb_image_text_8(
			xserver_get_conn(), 
			strlen(wnd->name),
			tb_wnd->handle, 
			gc, 
			wnd->toggle_btn->x, wnd->toggle_btn->y,
			wnd->toggle_btn->text);
}

bool taskbar_init(uint32_t bottom_offset) {
	struct window *r_wnd = xserver_get_root_wnd();
	uint32_t v_mask;
	uint32_t v_list[2];

	if(r_wnd == NULL) {
		return false;
	}

	tb_wnd = window_create(xcb_generate_id(xserver_get_conn()));
	
	window_setsize(tb_wnd, r_wnd->w, TASKBAR_HEIGHT);
	window_setcoords(tb_wnd, 0, r_wnd->h - tb_wnd->h - bottom_offset);

	v_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	v_list[0] = xserver_screen_get_white();
	v_list[1] = XCB_EVENT_MASK_BUTTON_PRESS;

	xcb_create_window(
			xserver_get_conn(),
			xserver_screen_get_depth(),
			tb_wnd->handle,
			r_wnd->handle,
			tb_wnd->x, /* pos X */
			tb_wnd->y, /* pos Y */
			tb_wnd->w, /* width */
			tb_wnd->h, /* height */
			1, /* border width */
			XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class */
			xserver_screen_get_visual(),
			v_mask, /* v-mask */
			v_list); /* v-list */

	xserver_map_window(tb_wnd);

	xserver_flush_conn();

	bg_gc_click = xserver_create_drawable_gc(
			xserver_screen_get_white(), xserver_screen_get_black(), tb_wnd);

	bg_gc_noclick = xserver_create_drawable_gc(
			xserver_screen_get_white(), xserver_screen_get_white(), tb_wnd);

	return true;
}

struct window *taskbar_get_window() {
	return tb_wnd;
}

void taskbar_cleanup() {
	xcb_free_gc(xserver_get_conn(), bg_gc_click);
	xcb_free_gc(xserver_get_conn(), bg_gc_noclick);
	window_destroy(tb_wnd);
}

uint32_t taskbar_get_width() {
	if(tb_wnd == NULL)
		return 0;

	return tb_wnd->w;
}

uint32_t taskbar_get_height() {
	if(tb_wnd == NULL)
		return 0;

	return tb_wnd->h;
}

void taskbar_update() {
	struct linked_list *all = wm_get_windows();
	struct window *wnd;

	uint32_t n = linked_list_count(all);
	uint32_t item_w = taskbar_get_width() / n;
	uint32_t item_h = taskbar_get_height();
	uint32_t offset = 0;
	uint32_t item_y = TASKBAR_BUTTON_YOFFSET;

	linked_list_rewind(all);

	xcb_gcontext_t fgc_noclick = xserver_create_font_gc(
			xserver_screen_get_white(),
			xserver_screen_get_black(),
			"fixed",
			tb_wnd);

	xcb_gcontext_t fgc_click = xserver_create_font_gc(
			xserver_screen_get_black(),
			xserver_screen_get_white(),
			"fixed",
			tb_wnd);

	xcb_clear_area(
			xserver_get_conn(), 1, tb_wnd->handle,
			0, 0,
			taskbar_get_width(),
			taskbar_get_height());

	printf("updating taskbar \n");

	while((wnd = linked_list_next(all)) != NULL) {
		/* create button if never created */
		if(wnd->toggle_btn == false)
			setup_window_btn(wnd, offset, item_y, item_w, item_h);
		else { /* else update with new dimensions */
			button_setcoords(wnd->toggle_btn, offset, item_y);
			button_setsize(wnd->toggle_btn, item_w, item_h);
		}

		draw_button(wnd, fgc_noclick, fgc_click);

		offset += item_w;

	}

	xcb_free_gc(xserver_get_conn(), fgc_noclick);
	xcb_free_gc(xserver_get_conn(), fgc_click);
	xserver_flush_conn();
}

void taskbar_onclick(uint32_t x, uint32_t y) {
	printf("taskbar: click at %dx%d\n", x, y);
	struct window *wnd;
	struct linked_list *all = wm_get_windows();
	linked_list_rewind(all);

	y += TASKBAR_BUTTON_YOFFSET;

	while((wnd = linked_list_next(all)) != NULL) {
		if(wnd->toggle_btn == NULL)
			continue;

		if(button_coord_in_bounds(wnd->toggle_btn, x, y)) {
			printf("in bounds !\n");
			button_onclick(wnd->toggle_btn, wnd);
			taskbar_update();
			break;
		} else {
			printf("not in bounds :(\n");
		}

	}
}
