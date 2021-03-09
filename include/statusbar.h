#ifndef _STATUSBAR_H
#define _STATUSBAR_H

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <pthread.h>
#include <unistd.h>
#include "general.h"
#include "xserver.h"
#include "wm.h"
#include "linked_list.h"
#include "widget.h"
#include "str.h"

#define STATUSBAR_HEIGHT 32
#define STATUSBAR_UPDATE_INTERVAL 1
#define STATUSBAR_PADDING_LEFT 16

bool statusbar_init();
void statusbar_cleanup();
void statusbar_update();
struct window *statusbar_get_window();
uint32_t statusbar_get_width();
uint32_t statusbar_get_height();
void statusbar_onclick(uint32_t x, uint32_t y);
void statusbar_register_widget(struct widget *wdg);
void statusbar_unregister_widget(struct widget *wdg);
bool statusbar_start_update_thread();
void statusbar_stop_update_thread();
#endif /* _STATUSBAR_H */
