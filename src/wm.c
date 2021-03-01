#include "wm.h"

static struct linked_list *wm_list = NULL;

void wm_init() {
	wm_list = linked_list_create();
}

void wm_register(struct window *wnd) {
	if(wnd == NULL) {
		return;
	}

	/* read wnd title */
	xcb_get_property_cookie_t g_cookie = xcb_get_property(
			xserver_get_conn(), 0,
			wnd->handle, XCB_ATOM_WM_NAME,XCB_ATOM_STRING, 0, 252/4);
	xcb_get_property_reply_t *p_reply = xcb_get_property_reply(
			xserver_get_conn(), g_cookie, NULL);

	if(xcb_get_property_value_length(p_reply) == 0) {
		printf("err: unable to read wnd title\n");
	} else {
		window_setname(wnd, (char *)xcb_get_property_value(p_reply));
	}

	free(p_reply);

	/* add to our list */
	linked_list_add(wm_list, wnd);

	printf("window added with title: %s\n", wnd->name);
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
	struct window *r_wnd = xserver_get_root_wnd();

	if(r_wnd == NULL)
		return;

	uint32_t c_mask = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
			XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;

	uint32_t c_values[4];
	uint32_t n_visible = 0;

	linked_list_rewind(wm_list);

	while((wnd = linked_list_next(wm_list)) != NULL) {
		if(window_is_visible(wnd))
			n_visible++;
	}

	printf("n_visible:%d\n", n_visible);

	if(n_visible == 0)
		return;


	uint32_t cells_h = (uint32_t)ceil(sqrt(n_visible));
	uint32_t cells_v = cells_h;

	uint32_t wnd_w = r_wnd->w / cells_h;
	uint32_t wnd_h = (r_wnd->h - taskbar_get_height()) / cells_v;

	linked_list_rewind(wm_list);

	for(int cx = 0; cx < cells_h; cx++) {
		for(int cy = 0; cy < cells_v; cy++) {
			while((wnd = linked_list_next(wm_list))) {
				if(wnd == NULL || window_is_visible(wnd))
					break;
			}

			if(wnd == NULL)
				break;

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

struct linked_list *wm_get_windows() {
	return wm_list;
}
