#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>

#include "general.h"
#include "wm.h"
#include "statusbar.h"
#include "taskbar.h"

int main() {
	int32_t ret = EXIT_SUCCESS;
	bool wm_running = true;

	wm_init();
	
	if(!xserver_connect()) {
		printf("x connection err!\n");
		ret = EXIT_FAILURE;
		goto _main_cleanup;
	}

	if(!xserver_init()) {
		ret = EXIT_FAILURE;
		goto _main_cleanup;
	}

	if(!statusbar_init()) {
		ret = EXIT_FAILURE;
		goto _main_cleanup;
	}

	if(!taskbar_init(statusbar_get_height())) {
		ret = EXIT_FAILURE;
		goto _main_cleanup;
	}

	xcb_key_symbols_t *k_syms = xcb_key_symbols_alloc(xserver_get_conn());

	xcb_generic_event_t *ev;
	xcb_key_press_event_t *kev;
	xcb_button_release_event_t *bev;

	struct window *wnd;

	while((ev = xcb_wait_for_event(xserver_get_conn())) && wm_running) {
		switch(ev->response_type) {
		case XCB_CONFIGURE_REQUEST:
			printf("--- config request! ---\n");
			xserver_handle_configure_request(
					(xcb_configure_request_event_t *)ev);
			break;
		case XCB_CONFIGURE_NOTIFY:
			printf("--- configure notify request! ---\n");
			break;
		case XCB_MAP_REQUEST:
			printf("--- map request! ---\n");
			wnd = window_create(((xcb_map_request_event_t *)ev)->window);
			wm_register(wnd);
			xserver_map_window(wnd);

			//wm_update();
			taskbar_update();
			break;
		case XCB_DESTROY_NOTIFY:
			printf("--- xcb destroy --\n");
			wm_unregister(
					wm_find_window(((xcb_destroy_notify_event_t *)ev)->window));
			wm_update();
			taskbar_update();
			break;
		case XCB_UNMAP_NOTIFY:
			printf("--- unmap notify! ---\n");
			wm_update();
			break;
		case XCB_MAP_NOTIFY:
			printf("--- map notify! ---\n");
			wm_update();
			break;
		case XCB_BUTTON_PRESS:
/*		case XCB_BUTTON_RELEASE: */
			printf("mouse button release \n");
			bev = (xcb_button_release_event_t *)ev;
			wnd = taskbar_get_window();

			if(wnd != NULL && bev->event == wnd->handle) {
				printf("pressed on taskbar @ %d,%d\n",
						bev->event_x, bev->event_y);
				taskbar_onclick(bev->event_x, bev->event_y);
			}

			printf("pressed mouse\n");
			break;
		case XCB_KEY_RELEASE:
			printf("--- key release! ---\n");
			break;

		case XCB_CREATE_NOTIFY:
			printf("--- create notify ---\n");
			break;

		case XCB_KEY_PRESS:
			kev = (xcb_key_press_event_t *)ev;
			xcb_keysym_t k_sym = xcb_key_symbols_get_keysym(
					k_syms, kev->detail, 0);

			switch(k_sym) {
			case 'q':
				printf("got signal to quit\n");
				wm_running = false;
				break;
			case 'x':
				printf("starting xterm\n");
				system("xcalc &");
				break;
			case 'y':
				printf("starting xterm\n");
				system("xterm &");
				break;
			case 'k':
				system("xkill &");
				break;
			default:	
				printf("got key %d (sym: %d)\n", *xcb_key_symbols_get_keycode(k_syms, k_sym), k_sym); 
			}
			break;
		default:
			printf("missed event: %d\n", ev->response_type);
			break;
		}
	}

_main_cleanup:
	wm_cleanup();
	statusbar_cleanup();
	taskbar_cleanup();
	xcb_disconnect(xserver_get_conn());
	return ret;
}
