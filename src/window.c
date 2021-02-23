#include "window.h"

struct window *window_create(const xcb_window_t handle) {
	struct window *wnd = malloc(sizeof(struct window));

	if(wnd == NULL) {
		return NULL;
	}
	
	wnd->handle = handle;
	wnd->x = 0;
	wnd->y = 0;
	wnd->w = 0;
	wnd->h = 0;

	return wnd;
}

void window_destroy(struct window *wnd) {
	free(wnd);
}
