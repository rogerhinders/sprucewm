#ifndef _WIDGET_H
#define _WIDGET_H

#include <string.h>
#include "general.h"

struct widget {
	char *shell_cmd;
};

struct widget *widget_create();
void widget_destroy(struct widget *wdg);
void widget_set_shell_cmd(struct widget *wdg, const char *cmd);

#endif /* _WIDGET_H */
