#include "xserver.h"

static xcb_connection_t *x_conn = NULL;
static struct window *root_wnd = NULL;
static uint32_t screen_col_white;
static uint32_t screen_col_black;
static uint8_t screen_color_depth;
static xcb_visualid_t screen_visual;

bool xserver_connect() {
	int err;

	x_conn = xcb_connect(NULL, NULL);

	if((err = xcb_connection_has_error(x_conn)) != 0) {
		x_conn = NULL;
		return false;
	}

	return true;
}

xcb_connection_t *xserver_get_conn() {
	return x_conn;
}

bool xserver_init() {
	uint32_t root_attr_name;
	uint32_t root_attr_value;

	if(x_conn == NULL) {
		printf("init err: no X connection detected!\n");
		return false;
	}

	root_attr_name = XCB_CW_EVENT_MASK;
	root_attr_value = XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
			XCB_EVENT_MASK_STRUCTURE_NOTIFY |
			XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
			XCB_EVENT_MASK_PROPERTY_CHANGE |
			XCB_EVENT_MASK_FOCUS_CHANGE |
			XCB_EVENT_MASK_ENTER_WINDOW |
			XCB_EVENT_MASK_LEAVE_WINDOW;

	xcb_screen_t *screen = xcb_setup_roots_iterator(
			xcb_get_setup(x_conn)).data;

	screen_col_black = screen->black_pixel;
	screen_col_white = screen->white_pixel;
	screen_color_depth = screen->root_depth;
	screen_visual = screen->root_visual;

	root_wnd = window_create(screen->root);

	window_setsize(
			root_wnd, screen->width_in_pixels, screen->height_in_pixels);

	xcb_change_window_attributes(
			x_conn, root_wnd->handle, root_attr_name, &root_attr_value);

	/* grab keys for WM modifier commands */
	xcb_grab_key(
			xserver_get_conn(),
			1,
			xserver_get_root_wnd()->handle,
			XCB_MOD_MASK_4,
			XCB_GRAB_ANY,
			XCB_GRAB_MODE_ASYNC,
			XCB_GRAB_MODE_ASYNC);

	xcb_flush(xserver_get_conn());

	return true;
}

struct window *xserver_get_root_wnd() {
	return root_wnd;
}

void xserver_map_window(struct window *wnd) {
	if(x_conn == NULL) {
		return;
	}

	xcb_map_window(x_conn, wnd->handle);
	xserver_flush_conn();
}

void xserver_unmap_window(struct window *wnd) {
	if(x_conn == NULL) {
		return;
	}

	xcb_unmap_window(x_conn, wnd->handle);
	xserver_flush_conn();
}

void xserver_flush_conn() {
	if(x_conn == NULL) {
		return;
	}

	xcb_flush(x_conn);
}

void xserver_handle_configure_request(xcb_configure_request_event_t *ev) {
	uint32_t val[7];
	uint32_t vi = 0;

	if(ev->value_mask & XCB_CONFIG_WINDOW_X) {
		val[vi++] = ev->x;
	}

	if(ev->value_mask & XCB_CONFIG_WINDOW_Y) {
		val[vi++] = ev->y;
	}

	if(ev->value_mask & XCB_CONFIG_WINDOW_WIDTH) {
		val[vi++] = ev->width;
	}

	if(ev->value_mask & XCB_CONFIG_WINDOW_HEIGHT) {
		val[vi++] = ev->height;
	}

	if(ev->value_mask & XCB_CONFIG_WINDOW_BORDER_WIDTH) {
		val[vi++] = ev->border_width;
	}

	if(ev->value_mask & XCB_CONFIG_WINDOW_SIBLING) {
		val[vi++] = ev->sibling;
	}

	if(ev->value_mask & XCB_CONFIG_WINDOW_STACK_MODE) {
		val[vi++] = ev->stack_mode;
	}

	xcb_configure_window(xserver_get_conn(), ev->window, ev->value_mask, val);
	xserver_flush_conn();
}

uint32_t xserver_screen_get_black() {
	return screen_col_black;
}

uint32_t xserver_screen_get_white() {
	return screen_col_white;
}

uint8_t xserver_screen_get_depth() {
	return screen_color_depth;
}

xcb_visualid_t xserver_screen_get_visual() {
	return screen_visual;
}

xcb_gcontext_t xserver_create_font_gc(
		uint32_t bg, uint32_t fg, char *font, struct window *wnd) {
	xcb_font_t font_id = xcb_generate_id(xserver_get_conn());

	xcb_open_font(
			xserver_get_conn(),
			font_id,
			strlen(font),
			font);

	xcb_gcontext_t gc = xcb_generate_id(xserver_get_conn());

	uint32_t f_mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND
			| XCB_GC_FONT;
	uint32_t f_vlist[3] = { fg, bg, font_id };

	xcb_create_gc(xserver_get_conn(), gc, wnd->handle, f_mask, f_vlist);

	xcb_close_font(xserver_get_conn(), font_id);

	return gc;
}

xcb_gcontext_t xserver_create_drawable_gc(
		uint32_t bg, uint32_t fg, struct window *wnd) {
	xcb_gcontext_t gc = xcb_generate_id(xserver_get_conn());
	uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
	uint32_t vlist[2] = { fg, bg };
	xcb_create_gc(xserver_get_conn(), gc, wnd->handle, mask, vlist);

	return gc;
}
