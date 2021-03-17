#ifndef _WM_H
#define _WM_H

#include <xcb/xcb.h>
#include <math.h>
#include "general.h"
#include "window.h"
#include "linked_list.h"
#include "xserver.h"
#include "taskbar.h"

#define WM_CONF_MASK XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | \
		XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT
#define WM_CONF_MASK_DOCK XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y
#define WM_ATTR_VALUES XCB_EVENT_MASK_FOCUS_CHANGE | \
		XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_PROPERTY_CHANGE

void wm_init();
void wm_register(struct window *wnd);
void wm_unregister(struct window *wnd);
struct window *wm_find_window(xcb_window_t handle);
void wm_update();
void wm_cleanup();
struct linked_list *wm_get_windows();
struct window *wm_get_current_focus();
void wm_set_current_focus(struct window *wnd);
void wm_refresh_focus();
void wm_check_ewmh(struct window *wnd);
void wm_hide_docked();
void wm_show_docked();

#endif /* _WM_H */
