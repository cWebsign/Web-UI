#pragma once

#include <str.h>
#include <arr.h>
#include <map.h>

#include <Net/web.h>

typedef struct WebUI {
	char 	*Route;
	Control **Controls;
	long 	ControlCount;
	CSS 	**Style;
	long 	StyleCount;

    int     (*Append)   	(struct WebUI *w, Control *newc);
    int     (*AppendCSS)	(struct WebUI *w, CSS *new_css);
} WebUI;

WebUI *InitManager(char *route_name);
int wAppendControl(WebUI *w, Control *newc);
int wAppendCSS(WebUI *w, CSS *new_css);
int wEditControl(WebUI *w, Control *newc);
char *FindKey(Map *m, const char *key);