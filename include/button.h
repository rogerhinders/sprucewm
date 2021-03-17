#ifndef _BUTTON_H
#define _BUTTON_H

#include <xcb/xcb.h>
#include <string.h>
#include "general.h"
#include "window.h"
#include "xserver.h"

#define BUTTON_TEXT_MAXLEN 255

struct button {
	struct window *parent;
	char text[BUTTON_TEXT_MAXLEN];
	bool pressed;
	void (*click_handler)(bool, void *);
	xcb_gcontext_t bg_gc_active;
	xcb_gcontext_t bg_gc_inactive;
	xcb_gcontext_t font_gc_active;
	xcb_gcontext_t font_gc_inactive;
	xcb_rectangle_t rect;
	uint32_t text_padding_left;
	uint32_t text_padding_top;
};

struct button *button_create();
void button_destroy(struct button *button);
void button_set_text_padding(struct button *btn,
		uint32_t top, uint32_t left, uint32_t bottom, uint32_t right);
void button_setcoords(struct button *button, uint32_t x, uint32_t y);
void button_setsize(struct button *button, uint32_t w, uint32_t h);
void button_settext(struct button *button, char *text);
void button_onclick(struct button *button, void *arg);
void button_setpressed(struct button *button, bool pressed);
void button_setclick_handler(
		struct button *button, void (*click_handler)(bool, void *));
bool button_coord_in_bounds(
		struct button *button, uint32_t x, uint32_t y);
void button_draw(struct button *btn);

#endif /* _BUTTON_H */
