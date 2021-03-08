#ifndef _STATUSBAR_H
#define _STATUSBAR_H

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include "general.h"
#include "xserver.h"
#include "wm.h"
#include "linked_list.h"

#define STATUSBAR_HEIGHT 32

bool statusbar_init();
void statusbar_cleanup();
void statusbar_update();
struct window *statusbar_get_window();
uint32_t statusbar_get_width();
uint32_t statusbar_get_height();
void statusbar_onclick(uint32_t x, uint32_t y);
#endif /* _STATUSBAR_H */
