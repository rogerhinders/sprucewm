#ifndef _XSERVER_H
#define _XSERVER_H

#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include "general.h"
#include "window.h"

bool xserver_connect();
xcb_connection_t *xserver_get_conn();
bool xserver_init();
struct window *xserver_get_root_wnd();
void xserver_map_window(struct window *wnd);
void xserver_flush_conn();
void xserver_handle_configure_request(xcb_configure_request_event_t *ev);

#endif /* _XSERVER_H */