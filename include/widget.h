#ifndef _WIDGET_H
#define _WIDGET_H

#include <string.h>
#include "general.h"
#include "str.h"

#define WIDGET_MAX_LEN 100

struct widget {
	char *shell_cmd;
	char *text;
};

struct widget *widget_create(const char *shell_cmd);
void widget_destroy(struct widget *wdg);
void widget_set_shell_cmd(struct widget *wdg, const char *cmd);
bool widget_update(struct widget *wdg);

#endif /* _WIDGET_H */
