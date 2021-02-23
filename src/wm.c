#include "wm.h"

static struct linked_list *wm_list = NULL;

void wm_init() {
	wm_list = linked_list_create();
}

void wm_register(struct window *wnd) {
	linked_list_add(wm_list, wnd);
}

void wm_update() {
	linked_list_rewind(wm_list);

	struct window *wnd;

	uint32_t c_values[] = {
		50, 50, 200, 200
	};

	uint32_t c_mask = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
			XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;

	while((wnd = linked_list_next(wm_list)) != NULL) {
		xcb_configure_window(xserver_get_conn(), wnd->handle, c_mask, c_values);
	}
}

void wm_cleanup() {
	linked_list_destroy(wm_list);
}
