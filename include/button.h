#ifndef _BUTTON_H
#define _BUTTON_H

#include <string.h>
#include "general.h"

#define BUTTON_TEXT_MAXLEN 255

struct button {
	char text[BUTTON_TEXT_MAXLEN];
	uint32_t x, y, w, h;
	bool pressed;
	void (*click_handler)(bool, void *);
};

struct button *button_create();
void button_destroy(struct button *button);
void button_setcoords(struct button *button, uint32_t x, uint32_t y);
void button_setsize(struct button *button, uint32_t w, uint32_t h);
void button_settext(struct button *button, char *text);
void button_onclick(struct button *button, void *arg);
void button_setpressed(struct button *button, bool pressed);
void button_setclick_handler(
		struct button *button, void (*click_handler)(bool, void *));
bool button_coord_in_bounds(
		struct button *button, uint32_t x, uint32_t y);

#endif /* _BUTTON_H */
