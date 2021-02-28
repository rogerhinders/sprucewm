#include "button.h"

static void default_clickhandler(bool clicked, void *arg) {

}

struct button *button_create() {

	struct button *btn = malloc(sizeof(struct button));

	button_settext(btn, "");
	button_setcoords(btn, 0, 0);
	button_setsize(btn, 0, 0);
	button_setpressed(btn, false);
	button_setclick_handler(btn, default_clickhandler);

	return btn;
}

void button_destroy(struct button *button) {
	free(button);
}

void button_setcoords(struct button *button, uint32_t x, uint32_t y) {
	button->x = x;
	button->y = y;
}

void button_setsize(struct button *button, uint32_t w, uint32_t h) {
	button->w = w;
	button->h = h;
}

void button_settext(struct button *button, char *text) {
	strncpy(button->text, text, BUTTON_TEXT_MAXLEN);
}

void button_setpressed(struct button *button, bool pressed) {
	button->pressed = pressed;
}

void button_setclick_handler(
		struct button *button, void (*click_handler)(bool, void *)) {
	button->click_handler = click_handler;
}

void button_onclick(struct button *button, void *arg) {
	button->pressed = !button->pressed;

	button->click_handler(button->pressed, arg);
}

bool button_coord_in_bounds(
		struct button *button, uint32_t x, uint32_t y) {
	printf("x: %d - %d y: %d - %d\n",
			button->x, button->x+button->w,
			button->y, button->y+button->h);
	return (x > button->x &&
			x < (button->x + button->w) &&
			y > button->y &&
			y < (button->y + button->h));
}
