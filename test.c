#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <str.h>
#include <map.h>
#include <Net/web.h>

CSS BodyCSS = (CSS){ .Class = "body", .Selector = 1, .Data = (char *[]){
    "background-color: #000",
    "color: #fff",
    NULL
}};

CSS txt_test = (CSS){ .Class = "txt_test", .Selector = 1, .Data = (char *[]){
    "color: #000",
    NULL
}};

Control Header = (Control){ .Tag = HEAD_TAG, .SubControlCount = 1, .SubControls = (void *[]){
    &(Control){ .Tag = TITLE_TAG, .Text = "Hello World" },
    NULL
}};

Control Body = (Control){ .Tag = BODY_TAG, .SubControlCount = 1, .SubControls = (void *[]){
    &(Control){ .Tag = P_TAG, .ID = "nigbob", .Class = "txt_test", .Text = "Hello World!" },
    &(Control){ .Tag = P_TAG, .ID = "luz", .Class = "txt_test", .Text = "Hi From C" },
    NULL
}};

char *FindKey(Map *m, const char *key) {
    if(!m || !m->arr)
        return NULL;

    for(int i = 0; i < m->idx; i++) {
        if(!m->arr[i])
            break;

        if(!strcmp(((jKey *)m->arr[i])->key, key))
            return ((jKey *)m->arr[i])->value;

        if(strstr(((jKey *)m->arr[i])->key, key))
            return ((jKey *)m->arr[i])->value;
    }

    return NULL;
}

void test_page(cWS *server, cWR *req, WebRoute *route, int sock) {
    if(req->RequestType.Is(&req->RequestType, "POST")) {
        /* New Control */
        Control *newc = CreateControl(P_TAG, NULL, NULL, "Websign :)", NULL);
        free(newc->CSS);
        newc->CSS = NULL;
        String t = newc->Construct(newc, 0, 1);

        /* Construct New Control */
        int mouse_x = atoi(((jKey *)req->Event.arr[6])->value);
        int mouse_y = atoi(((jKey *)req->Event.arr[7])->value);

        int scrn_x = atoi(((jKey *)req->Event.arr[8])->value);
        int scrn_y = atoi(((jKey *)req->Event.arr[9])->value);
        
        printf("Mouse: %d %d | Window Size: %d %d | Final: %d\n", mouse_x, mouse_y, scrn_x, scrn_y);

        String test = NewString("{\"update_styles\": { \".txt_test\": {\"margin-left\": \"");
        test.AppendNum(&test, mouse_x - (strlen("nigbob") * 3));
        test.AppendArray(&test, (const char *[]){"px\",", "\"margin-top\": \"", NULL});
        test.AppendNum(&test, mouse_y);
        test.AppendArray(&test, (const char *[]){"px\"}},", "\"replace_elements\": {\"nigbob\": \"", t.data, "\"}}", NULL});
        test.data[test.idx] = '\0';
        
        SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), test.data);
        
        test.Destruct(&test);
        t.Destruct(&t);
        newc->Destruct(newc, 0, 0);
        return;
    }

    char *template = ConstructTemplate((Control *[]){&Header, &Body, NULL}, (CSS *[]){&BodyCSS, NULL}, 1, 0, 0, 0, 0);
    if(!template)
    {
        printf("[ - ] Error, Unable to construct template...!\n");
        SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), "Error");
        return;
    }

    SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), template);
    free(template);
}

int main() {
    cWS *server = StartWebServer(NewString(""), 80, 0);
    if(!server)
    {
        printf("[ - ] Error, Unable to start web server....");
        return 1;
    }

    WebRoute *routes[] = {
        CreateRoute("index", "/", test_page),
        NULL
    };

    EnableLiveHandler(server);
    server->AddRoutes(server, routes);
    server->Run(server, 999, 0);
    server->Destruct(server);
    return 0;
}