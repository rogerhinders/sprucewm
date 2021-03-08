#ifndef _TASKBAR_H
#define _TASKBAR_H

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include "general.h"
#include "xserver.h"
#include "wm.h"

/* #define TASKBAR_HEIGHT_PERCENT 0.10 */ 
#define TASKBAR_HEIGHT 64
#define TASKBAR_BUTTON_YOFFSET 14

bool taskbar_init(uint32_t bottom_offset);
struct window *taskbar_get_window();
void taskbar_cleanup();
uint32_t taskbar_get_width();
uint32_t taskbar_get_height();
void taskbar_update();
void taskbar_onclick(uint32_t x, uint32_t y);

#endif /* _TASKBAR_H */
