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

	xcb_change_window_attributes(
			x_conn, root_wnd->handle, root_attr_name, &root_attr_value);

	return true;
}

struct window *xserver_get_root_wnd() {
	return root_wnd;
}

void xserver_map_window(struct window *wnd) {
	xcb_map_window(x_conn, wnd->handle);
	xcb_flush(x_conn);
}
