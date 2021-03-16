#ifndef _WINDOW_H
#define _WINDOW_H

#include <xcb/xcb.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xproto.h>
#include <string.h>
#include "xserver.h"
#include "general.h"
#include "button.h"

#define WINDOW_NAME_MAXLEN 255

struct window {
	xcb_window_t handle;
	uint32_t x, y, w, h;
	char name[WINDOW_NAME_MAXLEN];
	struct button *toggle_btn;
	bool allow_input;
	bool is_docked;
};

struct window *window_create(xcb_window_t handle);
void window_destroy(struct window *wnd);
void window_set_dock(struct window *wnd, bool dock);
void window_setcoords(struct window *wnd, uint32_t x, uint32_t y);
void window_setsize(struct window *wnd, uint32_t w, uint32_t h);
void window_setname(struct window *wnd, const char *name);
void window_set_toggle_button(
		struct window *wnd, struct button *btn);
bool window_is_visible(struct window *wnd);
void window_set_wm_hints(
		struct window *wnd, const xcb_icccm_wm_hints_t *wm_hints);
#endif /* _WINDOW_H */
