#include "button.h"

static void default_clickhandler(bool clicked, void *arg) {

}

struct button *button_create(struct window *wnd) {
	struct button *btn = malloc(sizeof(struct button));
	btn->parent = wnd;

	button_settext(btn, "");
	button_setcoords(btn, 0, 0);
	button_setsize(btn, 0, 0);
	button_setpressed(btn, false);
	button_set_text_padding(btn, 0, 0, 0, 0);

	button_setclick_handler(btn, default_clickhandler);

	btn->bg_gc_active = xserver_create_drawable_gc(
			xserver_screen_get_white(), xserver_screen_get_black(), wnd);

	btn->bg_gc_inactive = xserver_create_drawable_gc(
			xserver_screen_get_black(), xserver_screen_get_white(), wnd);

	btn->font_gc_active = xserver_create_font_gc(
			xserver_screen_get_black(),
			xserver_screen_get_white(),
			"-misc-fixed-medium-r-normal--20-200-75-75-c-100-iso8859-15",
			wnd);

	btn->font_gc_inactive = xserver_create_font_gc(
			xserver_screen_get_white(),
			xserver_screen_get_black(),
			"-misc-fixed-medium-r-normal--20-200-75-75-c-100-iso8859-15",
			wnd);

	return btn;
}

void button_destroy(struct button *button) {
	xcb_free_gc(xserver_get_conn(), button->bg_gc_active);
	xcb_free_gc(xserver_get_conn(), button->bg_gc_inactive);
	xcb_free_gc(xserver_get_conn(), button->font_gc_active);
	xcb_free_gc(xserver_get_conn(), button->font_gc_inactive);
	free(button);
}

void button_set_text_padding(struct button *btn,
		uint32_t top, uint32_t left, uint32_t bottom, uint32_t right) {
	btn->text_padding_top = top;
	btn->text_padding_left = left;
}

void button_setcoords(struct button *button, uint32_t x, uint32_t y) {
	button->rect.x = x;
	button->rect.y = y;
}

void button_setsize(struct button *button, uint32_t w, uint32_t h) {
	button->rect.width = w;
	button->rect.height = h;
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

	if(button->click_handler == NULL)
		return;

	button->click_handler(button->pressed, arg);
	button_draw(button);

	xserver_flush_conn();
}

bool button_coord_in_bounds(
		struct button *button, uint32_t x, uint32_t y) {
	printf("x: %d - %d y: %d - %d\n",
			button->rect.x, button->rect.x+button->rect.width,
			button->rect.y, button->rect.y+button->rect.height);
	return (x > button->rect.x &&
			x < (button->rect.x + button->rect.width) &&
			y > button->rect.y &&
			y < (button->rect.y + button->rect.height));
}

void button_draw(struct button *btn) {
	xcb_poly_fill_rectangle(
			xserver_get_conn(),
			btn->parent->handle,
			btn->pressed ? btn->bg_gc_active : btn->bg_gc_inactive,
			1,
			&btn->rect);

	xcb_image_text_8(
			xserver_get_conn(),
			strlen(btn->text),
			btn->parent->handle,
			btn->pressed ? btn->font_gc_active : btn->font_gc_inactive,
			btn->rect.x + btn->text_padding_left,
			btn->rect.y + btn->text_padding_top,
			btn->text);
}	
