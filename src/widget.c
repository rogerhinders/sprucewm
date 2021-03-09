#include "widget.h"

struct widget *widget_create(const char *shell_cmd) {
	struct widget *wdg = malloc(sizeof(struct widget));

	if(wdg == NULL)
		return NULL;

	widget_set_shell_cmd(wdg, shell_cmd);

	return wdg;
}

void widget_destroy(struct widget *wdg) {
	if(wdg == NULL)
		return;

	free(wdg->shell_cmd);
	free(wdg->text);
	free(wdg);
}

void widget_set_shell_cmd(struct widget *wdg, const char *cmd) {
	wdg->shell_cmd = strdup(cmd);
}

void widget_set_text(struct widget *wdg, const char *text) {
	char *s = strdup(text);
	str_trim(s);
	wdg->text = s;
}

bool widget_update(struct widget *wdg) {
	FILE *fp;
	char output[WIDGET_MAX_LEN];
	bool ret = true;

	if((fp = popen(wdg->shell_cmd, "r")) == NULL) {
		printf("widget update: error running %s\n", wdg->shell_cmd);
		ret = false;
		goto _widget_update_cleanup;
	}

	if(fgets(output, sizeof(output), fp) == NULL) {
		printf("widget update: empty data running %s\n", wdg->shell_cmd);
		ret = false;
		goto _widget_update_cleanup;
	}

	if(wdg->text != NULL)
		free(wdg->text);

	widget_set_text(wdg, output);	

_widget_update_cleanup:
	if(fp != NULL)
		pclose(fp);
	
	return ret;
}
