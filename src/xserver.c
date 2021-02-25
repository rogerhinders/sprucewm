#include "xserver.h"

static xcb_connection_t *x_conn = NULL;
static struct window *root_wnd = NULL;

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
			XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT;

	xcb_screen_t *screen = xcb_setup_roots_iterator(
			xcb_get_setup(x_conn)).data;

	root_wnd = window_create(screen->root);

	root_wnd->w = screen->width_in_pixels;
	root_wnd->h = screen->height_in_pixels;

	xcb_change_window_attributes(
			x_conn, root_wnd->handle, root_attr_name, &root_attr_value);

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
