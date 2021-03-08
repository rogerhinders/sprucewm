#include "widget.h"

struct widget *widget_create() {
	struct widget *wdg = malloc(sizeof(struct widget));

	if(wdg == NULL)
		return NULL;

	widget_set_shell_cmd(wdg, "");

	return wdg;
}

void widget_destroy(struct widget *wdg) {
	if(wdg == NULL)
		return;

	free(wdg->shell_cmd);
	free(wdg);
}

void widget_set_shell_cmd(struct widget *wdg, const char *cmd) {
	wdg->shell_cmd = strdup(cmd);
}
