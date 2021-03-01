#ifndef _XSERVER_H
#define _XSERVER_H

#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <string.h>
#include "general.h"
#include "window.h"

bool xserver_connect();
xcb_connection_t *xserver_get_conn();
bool xserver_init();
struct window *xserver_get_root_wnd();
void xserver_map_window(struct window *wnd);
void xserver_unmap_window(struct window *wnd);
void xserver_flush_conn();
void xserver_handle_configure_request(xcb_configure_request_event_t *ev);
uint32_t xserver_screen_get_black();
uint32_t xserver_screen_get_white();
uint8_t xserver_screen_get_depth();
xcb_visualid_t xserver_screen_get_visual();
xcb_gcontext_t xserver_create_font_gc(
		uint32_t bg, uint32_t fg, char *font, struct window *wnd);
xcb_gcontext_t xserver_create_drawable_gc(
		uint32_t bg, uint32_t fg, struct window *wnd);

#endif /* _XSERVER_H */
