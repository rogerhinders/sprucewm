#include "window.h"

struct window *window_create(const xcb_window_t handle) {
	struct window *wnd = malloc(sizeof(struct window));

	if(wnd == NULL) {
		return NULL;
	}
	
	wnd->handle = handle;

	window_setcoords(wnd, 0, 0);
	window_setsize(wnd, 0, 0);

	return wnd;
}

void window_destroy(struct window *wnd) {
	free(wnd);
}

void window_setcoords(struct window *wnd, uint32_t x, uint32_t y) {
	wnd->x = x;
	wnd->y = y;
}

void window_setsize(struct window *wnd, uint32_t w, uint32_t h) {
	wnd->w = w;
	wnd->h = h;
}
