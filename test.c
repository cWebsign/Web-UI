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

CSS CursorStyle = (CSS){ .Class = "cursor_ptr", .Selector = 1, .Data = (char *[]){
    "position: absolute",
    "background-color: #000",
    "width: 5px",
    "height: 5px",
    NULL
}};

Control Header = (Control){ .Tag = HEAD_TAG, .SubControlCount = 1, .SubControls = (void *[]){
    &(Control){ .Tag = TITLE_TAG, .Text = "Hello World" },
    NULL
}};

Control Body = (Control){ .Tag = BODY_TAG, .SubControlCount = 1, .SubControls = (void *[]){
    &(Control){ .Tag = P_TAG, .ID = "lulz", .Class = "cursor_ptr", .Text = "Hi From C" },
    &(Control){ .Tag = P_TAG, .ID = "nigbob", .CSS = (char *[]){"position: absolute", NULL}, .CSSCount = 1, /* .Class = "cursor_ptr", */ .Text = "" },
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

int PAINT_MODE = 0;
int LAST_X = 0;
int LAST_Y = 0;

// void test_page(cWS *server, cWR *req, WebRoute *route, int sock) {
//     if(req->RequestType.Is(&req->RequestType, "POST")) {
//         /* New Control */
//         Control *newc = CreateControl(P_TAG, NULL, NULL, "Websign :)", NULL);
//         free(newc->CSS);
//         newc->CSS = NULL;
//         String t = newc->Construct(newc, 0, 1);

//         /* Construct New Control */
//         char *event = ((jKey *)req->Event.arr[1])->value;
//         int mouse_x = atoi(((jKey *)req->Event.arr[6])->value);
//         int mouse_y = atoi(((jKey *)req->Event.arr[7])->value);
//         printf("%s %d\n", event, *(int *)route->Args[0]);
//         if(strstr(event, "click") && !*(int *)route->Args[0]) {
//             printf("HERE\n");
//             *(int *)route->Args[0] = 1;
//             LAST_X = mouse_x;
//             LAST_Y = mouse_y;

//             String test = NewString("{\"update_styles\": { \".cursor_ptr\": {\"margin-left\": \"");
//             test.AppendNum(&test, LAST_X);
//             test.AppendArray(&test, (const char *[]){"px\",", "\"margin-top\": \"", NULL});
//             test.AppendNum(&test, LAST_Y);
//             test.AppendArray(&test, (const char *[]){"px\"}}}", NULL});
            
//             test.data[test.idx] = '\0';
            
//             SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), test.data);
//             test.Destruct(&test);
//             t.Destruct(&t);
//             newc->Destruct(newc, 0, 0);
//             return;
//         } else if(strstr(event, "click")) {
//             *(int *)route->Args[0] = 0;
//             return;
//         }

//         if(!PAINT_MODE) {
//             SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), "fetched");
//             newc->Destruct(newc, 0, 0);
//             t.Destruct(&t);
//             return;
//         }

//         int scrn_x = atoi(((jKey *)req->Event.arr[8])->value);
//         int scrn_y = atoi(((jKey *)req->Event.arr[9])->value);
        
//         int final_x = LAST_X > mouse_x ? LAST_X - mouse_x : mouse_x - LAST_X;
//         int final_y = LAST_Y > mouse_y ? LAST_Y - mouse_y : mouse_y - LAST_Y;

//         printf("Mouse: %d %d | Window Size: %d %d | Final: %d:%d\n", mouse_x, mouse_y, scrn_x, scrn_y, final_x, final_y);

//         String test = NewString("{\"update_styles\": { \".cursor_ptr\": {\"margin-left\": \"");
//         test.AppendNum(&test, LAST_X);
//         test.AppendArray(&test, (const char *[]){"px\",", "\"margin-top\": \"", NULL});
//         test.AppendNum(&test, LAST_Y);
//         test.AppendArray(&test, (const char *[]){"px\", \"width\": \"", NULL});
//         test.AppendNum(&test, final_x);
//         test.AppendArray(&test, (const char *[]){"\", \"height\": \"", NULL});
//         test.AppendNum(&test, final_y);
//         test.AppendArray(&test, (const char *[]){"\"}}}", /* "\"replace_elements\": {\"nigbob\": \"", t.data, "\"}}", */ NULL});
//         test.data[test.idx] = '\0';
        
//         SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), test.data);
        
//         test.Destruct(&test);
//         t.Destruct(&t);
//         newc->Destruct(newc, 0, 0);
//         return;
//     }

//     char *template = ConstructTemplate((Control *[]){&Header, &Body, NULL}, (CSS *[]){&BodyCSS, &CursorStyle, NULL}, 1, 0, 1, 0, 0);
//     if(!template)
//     {
//         printf("[ - ] Error, Unable to construct template...!\n");
//         SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), "Error");
//         return;
//     }

//     SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), template);
//     free(template);
// }

void test_page(cWS *server, cWR *req, WebRoute *route, int sock) {
    if(req->RequestType.Is(&req->RequestType, "POST")) {
        /* New Control */
        Control *newc = CreateControl(P_TAG, NULL, NULL, "Hi From cWebsign :)", NULL);
        free(newc->CSS);
        newc->CSS = NULL;
        String t = newc->Construct(newc, 0, 1);

        /* Construct New Control */
        char *event = ((jKey *)req->Event.arr[1])->value;
        char *mouse_x = ((jKey *)req->Event.arr[6])->value;
        char *mouse_y = ((jKey *)req->Event.arr[7])->value;
        printf("Clicked Position %s:%s\n", mouse_x, mouse_y);
        if(!strcmp(event, "click") && !atoi(mouse_x) && !atoi(mouse_y)) {
            printf("[ - ] Error, Fetching events...!\n");
            newc->Destruct(newc, 0, 0);
            SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), "Error!");
            return;
        } else if(!strcmp(event, "keydown") && req->Event.idx > 10) {
            char *key_pressed = ((jKey *)req->Event.arr[10])->value;
            // if(!strcmp(key_pressed)  )
        }

        String test = NewString("{\"update_styles\": { \".cursor_ptr\": {\"color\": \"#ff0000\", \"position\": \"absolute\"");
        test.AppendArray(&test, (const char *[]){",\"margin-top\": \"", mouse_y, "\", \"margin-left\": \"", mouse_x, "\"}},\"replace_elements\": {\"lulz\": \"", t.data, "\"}}", NULL});
        test.data[test.idx] = '\0';
        
        SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), test.data);
        
        test.Destruct(&test);
        t.Destruct(&t);
        newc->Destruct(newc, 0, 0);
        return;
    }

    char *template = ConstructTemplate((Control *[]){&Header, &Body, NULL}, (CSS *[]){&BodyCSS, &CursorStyle, NULL}, 1, 0, 0, 1, 0);
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
        &(WebRoute){ .Name = "index", .Path = "/", .Handler = test_page, .Args = (void *[]){&PAINT_MODE, NULL} },
        NULL
    };

    EnableLiveHandler(server);
    server->AddRoutes(server, routes);
    server->Run(server, 999, 0);
    server->Destruct(server);
    return 0;
}