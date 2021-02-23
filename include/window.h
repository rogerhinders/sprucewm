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

#endif /* _WINDOW_H */
