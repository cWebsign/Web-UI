#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../t.h"

WebUI *InitManager(char *route_name) {
	WebUI *w = (WebUI *)malloc(sizeof(WebUI));
	*w = (WebUI){
		.Route 			= strdup(route_name),
		.Controls 		= (Control **)malloc(sizeof(Control) * 1),
		.ControlCount 	= 0,
		.Style 			= (CSS **)malloc(sizeof(CSS) * 1),
		.StyleCount 	= 0,

		.Append 		= wAppendControl,
		.AppendCSS 		= wAppendCSS
	};

	return w;
}

int wAppendControl(WebUI *w, Control *newc) {
	if(!w || !newc)
		return 0;

	w->Controls[w->ControlCount] = newc;
	w->ControlCount++;
	w->Controls = (Control **)realloc(w->Controls, sizeof(Control *) * (w->ControlCount + 1));
	w->Controls[w->ControlCount] = NULL;

	return 1;
}

int wAppendCSS(WebUI *w, CSS *new_css) {
	if(!w || !new_css)
		return 0;

	w->Style[w->StyleCount] = new_css;
	w->StyleCount++;
	w->Style = (CSS **)realloc(w->Style, sizeof(CSS *) * (w->StyleCount + 1));
	w->Style[w->StyleCount] = NULL;

	return 1;
}

// Custom Map Function
char *FindKey(Map *m, const char *key) {
    if(!m || !m->arr)
        return NULL;

    for(int i = 0; i < m->idx; i++) {
        if(!m->arr[i])
            break;

        if(!strcmp(((Key *)m->arr[i])->key, key))
            return ((Key *)m->arr[i])->value;

        if(strstr(((Key *)m->arr[i])->key, key))
            return ((Key *)m->arr[i])->value;
    }

    return NULL;
}