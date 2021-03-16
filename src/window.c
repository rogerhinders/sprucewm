#include "window.h"

struct window *window_create(const xcb_window_t handle) {
	struct window *wnd = malloc(sizeof(struct window));

	if(wnd == NULL) {
		return NULL;
	}
	
	wnd->handle = handle;

	window_setcoords(wnd, 0, 0);
	window_setsize(wnd, 0, 0);
	window_setname(wnd, " ");
	window_set_toggle_button(wnd, NULL);
	window_set_dock(wnd, false);

	wnd->allow_input = true;

	return wnd;
}

void window_set_dock(struct window *wnd, bool dock) {
	wnd->is_docked = dock;
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

void window_setname(struct window *wnd, const char *name) {
	strncpy(wnd->name, name, WINDOW_NAME_MAXLEN);
}

void window_set_toggle_button(
		struct window *wnd, struct button *btn) {
	wnd->toggle_btn = btn;
}

bool window_is_visible(struct window *wnd) {
	return wnd->toggle_btn == NULL || wnd->toggle_btn->pressed;
}

void window_set_wm_hints(
		struct window *wnd, const xcb_icccm_wm_hints_t *wm_hints) {
	if(wm_hints->flags & XCB_ICCCM_WM_HINT_INPUT)
		wnd->allow_input = (bool)wm_hints->input;
}
