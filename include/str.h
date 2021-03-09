#ifndef _STR_H
#define _STR_H

#include <ctype.h>
#include <libgen.h>
#include <string.h>
#include "general.h"

void str_ltrim(char *s);
void str_rtrim(char *s);
void str_rtrim_special_only(char *s);
void str_trim(char *s);
void str_rtrim_slash(char *s);
void str_tolower(char *s);
/*char *str_concat(const char *a, const char *b);*/

#endif /* _STR_H */
