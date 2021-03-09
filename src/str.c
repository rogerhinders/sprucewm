#include "str.h"

void str_ltrim(char *s) {
	if(s == NULL) {
		return;
	}

	int shift_n = 0;
	char *ps = s;
	size_t len = strlen(s);

	if(len == 0) {
		return;
	}

	while(*ps <= 0x20 && *ps != '\0') {
		shift_n++;
		ps++;
	}

	memmove(s, s+shift_n, len-shift_n+1);
}

void __str_rtrim(char *s, bool trim_space) {
	size_t len = strlen(s);
	
	if(len == 0) {
		return;
	}

	s += len-1;

	char treshold = trim_space ? 0x1F : 0x20;

	while(*s <= treshold) {
		*s = '\0';
		s--;
		len--;
		if(len == 0) {
			break;
		}
	}
}

void str_rtrim(char *s) {
	if(s == NULL) {
		return;
	}

	__str_rtrim(s, false);
}

void str_rtrim_special_only(char *s) {
	if(s == NULL) {
		return;
	}

	__str_rtrim(s, true);
}

void str_trim(char *s) {
	str_ltrim(s);
	str_rtrim(s);
}

void str_rtrim_slash(char *s) {
	size_t len = strlen(s);

	str_trim(s);

	if(len == 0) {
		return;
	}

	if(s[len-1] == '/') {
		s[len-1] = '\0';
	}
}

void str_tolower(char *s) {
	if(s == NULL) {
		return;
	}

	if(strlen(s) == 0) {
		return;
	}

	for(int i = 0; s[i]; i++) {
		s[i] = tolower(s[i]);
	}
}

/*
char *str_concat(const char *a, const char *b) {
	if(a == NULL) {
		return NULL;
	}

	if(b == NULL) {
		return NULL;
	}

	size_t l_a = strlen(a);
	size_t l_b = strlen(b);
	size_t l_tot = l_a+l_b+1;
	char *r = malloc(l_tot);
	r[0] = '\0';

	strlcat(r, a, l_tot);
	strlcat(r, b, l_tot);

	return r;
}*/
