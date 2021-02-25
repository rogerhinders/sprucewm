#ifndef _WM_H
#define _WM_H

#include <xcb/xcb.h>
#include <math.h>
#include "general.h"
#include "window.h"
#include "linked_list.h"
#include "xserver.h"

void wm_init();
void wm_register(struct window *wnd);
void wm_unregister(struct window *wnd);
struct window *wm_find_window(xcb_window_t handle);
void wm_update();
void wm_cleanup();

#endif /* _WM_H */
