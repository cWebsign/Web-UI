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

Control Header = (Control){ .Tag = HEAD_TAG, .SubControlCount = 1, .SubControls = (void *[]){
    &(Control){ .Tag = TITLE_TAG, .Text = "Hello World" },
    NULL
}};

Control Body = (Control){ .Tag = BODY_TAG, .SubControlCount = 1, .SubControls = (void *[]){
    &(Control){ .Tag = P_TAG, .ID = "nigbob", .Text = "Hello World!" },
    &(Control){ .Tag = P_TAG, .ID = "lul", .Text = "Hi From C" },
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
        Control *newc = CreateControl(P_TAG, NULL, NULL, "nigbob", NULL);
        newc->CSS = NULL;
        String t = newc->Construct(newc, 0, 0);

        /* Construct New Control */
        String test = NewString("{\"replace_elements\": {\"nigbob\": \"");
        test.AppendArray(&test, (const char *[]){t.data, "\"}}", NULL});
        test.data[test.idx] = '\0';
        printf("%s\n", test.data);

        // String test = ChangeElement(1, (char *[]){
        //     {"nigbob", t.data},
        //     NULL
        // });
        
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

void event_handler(cWS *server, cWR *req, WebRoute *route, int sock) {
    if(req->RequestType.Is(&req->RequestType, "POST")) {
        if(FindKey(&req->Headers, "CF-Connecting-IP") || FindKey(&req->Headers, "cf-connecting-ip") || FindKey(&req->Headers, "x-forwarded-for"))
            fetch_cf_post_data(server, req, sock);
        
        Map json = Decode_OneLine_JSON(req->Body.data);
        char *route = FindKey(&json, "Route");
        if(!route) {
            printf("Error\n");
            return;
        }

        int chk = SearchRoute(server, route);
        if(chk > -1)
        {
            (void)(chk > -1 ? ((void (*)(cWS *, cWR *, WebRoute *, int))((WebRoute *)server->CFG.Routes[chk])->Handler)(server, req, server->CFG.Routes[chk], sock) : SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), "ERROR\n\n\n"));
            return;
        }

        SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), "ERROR\n\n\n");
    }
}

int main() {
    cWS *server = StartWebServer(NewString(""), 80, 0);
    if(!server)
    {
        printf("[ - ] Error, Unable to start web server....");
        return 1;
    }

    server->AddRoutes(server, (WebRoute *[]){
        &(WebRoute){ .Name = "index", .Path = "/", .Handler = test_page },
        &(WebRoute){ .Name = "event_handler", .Path = "/event_handler", .Handler = event_handler },
        NULL
    });

    server->Run(server, 999, 0);
    return 0;
}