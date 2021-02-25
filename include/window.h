#ifndef _WINDOW_H
#define _WINDOW_H

#include "general.h"
#include <xcb/xcb.h>

struct window {
	xcb_window_t handle;
	uint32_t x, y, w, h;
};

struct window *window_create(xcb_window_t handle);
void window_destroy(struct window *wnd);
void window_setcoords(struct window *wnd, uint32_t x, uint32_t y);
void window_setsize(struct window *wnd, uint32_t w, uint32_t h);

#endif /* _WINDOW_H */
