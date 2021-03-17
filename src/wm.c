#include "wm.h"

static struct linked_list *wm_list = NULL;
static struct window* current_focus = NULL;

static void set_docked_visibility(bool visible) {
	struct window *wnd;

	linked_list_rewind(wm_list);

	while((wnd = linked_list_next(wm_list)) != NULL) {
		if(!wnd->is_docked)
			continue;

		if(visible)
			xserver_map_window(wnd);
		else
			xserver_unmap_window(wnd);
	}

	xserver_flush_conn();
}

static uint32_t get_status_area_height() {
	return taskbar_get_height() + statusbar_get_height();
}

static uint32_t get_dock_area_height() {
	uint32_t h = 0;
	struct window *wnd;
	linked_list_rewind(wm_list);

	while((wnd = linked_list_next(wm_list)) != NULL)
		if(wnd->is_docked && window_is_visible(wnd))
			h += wnd->h;

	return h;
}

static uint32_t get_tile_area_height() {
	return xserver_get_root_wnd()->h - get_status_area_height()
		- get_dock_area_height();
}

static uint32_t count_visible_tiles(bool docked) {
	struct window *wnd;
	uint32_t n_visible = 0;
	linked_list_rewind(wm_list);

	while((wnd = linked_list_next(wm_list)) != NULL)
		if(window_is_visible(wnd) && wnd->is_docked == docked)
			n_visible++;
	
	return n_visible;
}

static void update_normal_tiles() {
	uint32_t n_tiles = count_visible_tiles(false);
	uint32_t wnd_w = xserver_get_root_wnd()->w;
	uint32_t wnd_h = get_tile_area_height() / n_tiles;
	struct window *wnd;
	uint32_t cur_y = 0;
	uint32_t c_values[4];
	uint32_t a_values = WM_ATTR_VALUES;

	linked_list_rewind(wm_list);

	while((wnd = linked_list_next(wm_list)) != NULL) {
		if(!window_is_visible(wnd) || wnd->is_docked)
			continue;

		window_setcoords(wnd, 0, cur_y);
		window_setsize(wnd, wnd_w, wnd_h);

		c_values[0] = wnd->x;
		c_values[1] = wnd->y;
		c_values[2] = wnd->w;
		c_values[3] = wnd->h;

		xcb_configure_window(
				xserver_get_conn(), wnd->handle, WM_CONF_MASK, c_values);

		xcb_change_window_attributes(
				xserver_get_conn(),
				wnd->handle,
				XCB_CW_EVENT_MASK,
				&a_values);

		cur_y += wnd_h;
	}
}

static void update_docked_tiles() {
	struct window *wnd;
	uint32_t cur_y = get_tile_area_height(); 
	uint32_t c_values[2];
	uint32_t a_values = WM_ATTR_VALUES;

	linked_list_rewind(wm_list);

	while((wnd = linked_list_next(wm_list)) != NULL) {
		if(!window_is_visible(wnd) || !wnd->is_docked)
			continue;

		window_setcoords(wnd, 0, cur_y);
		
		c_values[0] = wnd->x;
		c_values[1] = wnd->y;

		xcb_configure_window(
				xserver_get_conn(), wnd->handle, WM_CONF_MASK_DOCK, c_values);

		xcb_change_window_attributes(
				xserver_get_conn(),
				wnd->handle,
				XCB_CW_EVENT_MASK,
				&a_values);

		cur_y += wnd->h;
	}
}

void wm_init() {
	wm_list = linked_list_create();
}

void wm_register(struct window *wnd) {
	if(wnd == NULL) {
		return;
	}

	/* read wnd title */
	xcb_get_property_cookie_t g_cookie = xcb_get_property(
			xserver_get_conn(), 0,
			wnd->handle, XCB_ATOM_WM_NAME,XCB_ATOM_STRING, 0, 252/4);
	xcb_get_property_reply_t *p_reply = xcb_get_property_reply(
			xserver_get_conn(), g_cookie, NULL);

	if(xcb_get_property_value_length(p_reply) == 0) {
		printf("err: unable to read wnd title\n");
		window_setname(wnd, "<noname>");
	} else {
		window_setname(wnd, (char *)xcb_get_property_value(p_reply));
	}

	free(p_reply);

	/* add to our list */
	linked_list_add(wm_list, wnd);

	printf("window added with title: %s\n", wnd->name);
}

void wm_unregister(struct window *wnd) {
	if(wnd == NULL) {
		return;
	}

	if(current_focus == wnd)
		wm_set_current_focus(NULL);

	linked_list_remove(wm_list, wnd);
}

struct window *wm_find_window(xcb_window_t handle) {
	linked_list_rewind(wm_list);

	struct window *wnd = NULL;

	while((wnd = linked_list_next(wm_list)) != NULL) {
		if(wnd->handle == handle) {
			break;
		}
	}

	return wnd;
}

void wm_update() {
	if(xserver_get_root_wnd() == NULL)
		return;
	
	update_normal_tiles();
	update_docked_tiles();

	xserver_flush_conn();
}

void wm_cleanup() {
	linked_list_destroy(wm_list);
}

struct linked_list *wm_get_windows() {
	return wm_list;
}

void wm_set_current_focus(struct window *wnd) {
	current_focus = wnd;
}

struct window *wm_get_current_focus() {
	return current_focus;
}

void wm_refresh_focus() {
	if(current_focus == NULL)
		return;

	xcb_set_input_focus(
			xserver_get_conn(),
			XCB_INPUT_FOCUS_POINTER_ROOT,
			current_focus->handle,
			XCB_CURRENT_TIME);

	xserver_flush_conn();
}

void wm_check_ewmh(struct window *wnd) {
	char *s_type = "_NET_WM_WINDOW_TYPE";
	char *s_dock = "_NET_WM_WINDOW_TYPE_DOCK";

	xcb_intern_atom_reply_t *r_type, *r_dock;

	if(!(r_type = xcb_intern_atom_reply(
					xserver_get_conn(),
					xcb_intern_atom(
						xserver_get_conn(), 0, strlen(s_type), s_type),
					NULL)))
			return;

	if(!(r_dock = xcb_intern_atom_reply(
					xserver_get_conn(),
					xcb_intern_atom(
						xserver_get_conn(), 0, strlen(s_dock), s_dock),
					NULL)))
			return;

	xcb_get_property_cookie_t c = xcb_get_property(
			xserver_get_conn(),
			false,
			wnd->handle,
			r_type->atom,
			XCB_ATOM_ATOM,
			0,
			1);

	xcb_get_property_reply_t *r	= xcb_get_property_reply(
			xserver_get_conn(),
			c,
			NULL);

	if(!r || r->length == 0)
		return;

	xcb_atom_t *r_val;

	if(!(r_val = xcb_get_property_value(r)))
		return;

	if(*r_val == r_dock->atom) {
		printf("setting dock property\n");
		window_set_dock(wnd, true);
	}
}

void wm_hide_docked() {
	set_docked_visibility(false);
}

void wm_show_docked() {
	set_docked_visibility(true);
}

