#include "wm.h"

static struct linked_list *wm_list = NULL;

void wm_init() {
	wm_list = linked_list_create();
}

void wm_register(struct window *wnd) {
	if(wnd == NULL) {
		return;
	}

	linked_list_add(wm_list, wnd);
}

void wm_unregister(struct window *wnd) {
	if(wnd == NULL) {
		return;
	}

	linked_list_remove(wm_list, wnd);
}

struct window *wm_find_window(xcb_window_t handle) {
	linked_list_rewind(wm_list);

	struct window *wnd = NULL;

	while((wnd = linked_list_next(wm_list)) != NULL) {
		if(wnd->handle == handle) {
			break;
		}
	}

	return wnd;
}

void wm_update() {
	struct window *wnd;

	uint32_t c_mask = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
			XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;

	uint32_t c_values[4];

	uint32_t cells_h = (uint32_t)ceil(sqrt(linked_list_count(wm_list)));
	uint32_t cells_v = cells_h;

	uint32_t wnd_w = xserver_get_root_wnd()->w / cells_h;
	uint32_t wnd_h = xserver_get_root_wnd()->h / cells_v;

	linked_list_rewind(wm_list);

	for(int cx = 0; cx < cells_h; cx++) {
		for(int cy = 0; cy < cells_v; cy++) {
			wnd = linked_list_next(wm_list);

			if(wnd == NULL) {
				break;
			}

			window_setcoords(wnd, cx * wnd_w, cy * wnd_h);
			window_setsize(wnd, wnd_w, wnd_h);

			c_values[0] = wnd->x;
			c_values[1] = wnd->y;
			c_values[2] = wnd->w;
			c_values[3] = wnd->h;

			xcb_configure_window(
					xserver_get_conn(), wnd->handle, c_mask, c_values);
		}
	}

	xserver_flush_conn();
}

void wm_cleanup() {
	linked_list_destroy(wm_list);
}
