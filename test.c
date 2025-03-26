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

CSS MiniPanelStyle = (CSS){ .Class = "mini_panel", .Selector = 1, .Data = (char *[]){
    "position: absolute",
    "margin: auto",
    "top: 0",
    "right: 0",
    "background-color: #021437",
    "color: #fff",
    "border: 2px solid black",
    "border-radius: 15px",
    "position: absolute",
    "padding: 10px",
    "width: 500px",
    "height: 100%",
    NULL
}};

CSS NewCtrlBtn = (CSS){ .Class = "new_ctrl_btn", .Selector = 1, .Data = (char *[]){
    "background-color: green",
    "border-radius: 15px",
    "padding: 10px",
    "width: 385px",
    "text-align: center",
    "margin-top: 20px",
    NULL
}};

CSS NewCtrlLabel = (CSS){ .Class = "new_ctrl_label", .Selector = 1, .Data = (char *[]){
    "font-size: 18",
    NULL
}};


CSS NewCtrlTxtbox = (CSS){ .Class = "new_ctrl_txtbox", .Selector = 1, .Data = (char *[]){
    "border-radius: 15px",
    "width: 200px",
    "padding: 5px",
    "font-size: 17",
    NULL
}};

CSS ControlTable = (CSS){ .Class = "control_table", .Selector = 1, .Data = (char *[]){
    "margin-top: 10px",
    "display: grid",
    "grid-template-columns: 120px 120px 120px 120px",
    NULL
}};

CSS InvokeForm = (CSS){ .Class = "form > *", .Selector = 1, .Data = (char *[]){
    "display: block",
    NULL
}};

CSS DisplayInRow = (CSS){ .Class = "display_in_row", .Selector = 1, .Data = (char *[]){
    "display: inline-block",
    NULL
}};

Control Header = (Control){ .Tag = HEAD_TAG, .SubControlCount = 1, .SubControls = (void *[]){
    &(Control){ .Tag = TITLE_TAG, .Text = "Hello World" },
    NULL
}};

Control Body = (Control){ .Tag = BODY_TAG, .SubControlCount = 1, .SubControls = (void *[]){
    &(Control){ .Tag = DIV_TAG, .Class = "mini_panel", .SubControlCount = 10, .SubControls = (void *[]){ 
        &(Control){ .Tag = P_TAG, .CSS = (char *[]){"width: 100%;", "text-align: center;", "align-items: center;", NULL}, .CSSCount = 3, .Text = "Mini UI Panel" },
        &(Control){ .Tag = P_TAG, .Class = "display_in_row", .Text = "Editing Control: N/A" },
        &(Control){ .Tag = P_TAG, .Class = "new_ctrl_label", .Text = "Create a New Control" },
        &(Control){ .Tag = INPUT_TAG, .ID = "new_ctrl_tag_input", .Class = "new_ctrl_txtbox", .Type = "text", .Data = "placeholder=\"Tag\"" },
        &(Control){ .Tag = INPUT_TAG, .ID = "new_ctrl_class_input", .Class = "new_ctrl_txtbox", .Type = "text", .Data = "placeholder=\"Class\"" },
        &(Control){ .Tag = INPUT_TAG, .ID = "new_ctrl_id_input", .Class = "new_ctrl_txtbox", .Type = "text", .Data = "placeholder=\"ID\"" },
        &(Control){ .Tag = INPUT_TAG, .ID = "new_ctrl_text_input", .Class = "new_ctrl_txtbox", .Type = "text", .Data = "placeholder=\"Text\"" },
        &(Control){ .Tag = P_TAG, .Class = "new_ctrl_btn", .ID = "ws_form", .Text = "Add New Control" },
        &(Control){ .Tag = DIV_TAG, .Class = "control_table", .SubControlCount = 4, .SubControls = (void *[]){
            &(Control){ .Tag = DIV_TAG, .Class = "table_row", .Text = "Control Tag" },
            &(Control){ .Tag = DIV_TAG, .Class = "table_row", .Text = "ID" },
            &(Control){ .Tag = DIV_TAG, .Class = "table_row", .Text = "Class" },
            &(Control){ .Tag = DIV_TAG, .Class = "table_row", .Text = "Text" },
            NULL
        }},
        &(Control){ .Tag = P_TAG, .ID = "move_panel", .Text = "Move UI Panel"},
        NULL
    }},
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

char **GetDefaultCSS(CSS *style) {
    if(!style)
        return NULL;

    char **arr = (char **)malloc(sizeof(char *) * 1);
    for(int i = 0; style->Data[i] != NULL; i++) {
        arr[i] = style->Data[i];
        arr = (char **)realloc(arr, sizeof(char *) * (i + 1));
        arr[i] = NULL;
    }

    return arr;
}

char **CombineCSS(CSS *style, char **newc) {
    if(!style)
        return NULL;

    char **arr = GetDefaultCSS(style);
    int idx = 0;
    while(arr[idx] != NULL) idx++;

    for(int i = 0; newc[i] != NULL; i++) {
        arr[idx] = newc[i];
        idx++;
        arr = (char **)realloc(arr, sizeof(char *) * (idx + 1));
        arr[i] = NULL;
    }

    if(idx > 0)
        return arr;

    free(arr);
    return NULL;
}

int isClicked(Map event, char *id) {
    if(!event.arr)
        return 0;

    if(!strcmp(((jKey *)event.arr[1])->value, "click") && !strcmp(((jKey *)event.arr[3])->value, id))
        return 1;

    return 0;
}

int isKeyPressed(Map event, char *key, char *on_element_id) {
    if(!event.arr)
        return 0;

    return (!on_element_id ? !strcmp(((jKey *)event.arr[1])->value, "keyup") && !strcmp(event.arr[10], key) : !strcmp(((jKey *)event.arr[1])->value, "keyup") && !strcmp(event.arr[10], key) && !strcmp(event.arr[3], on_element_id));
}

/*
*/
void UpdateUI(cWS *server, cWR *req, Control *new_content, Control **controls, CSS **style) {
    String resp = NewString("{");

    if(new_content) {
        resp.AppendString(&resp, "\"new_body_content\": \"");

        String element = new_content->Construct(new_content, 0, 1);

        String__ReplaceChar(&element, '"', "'");
        while(element.FindChar(&element, '\t') != -1)
            element.Trim(&element, '\t');

        element.data[element.idx] = '\0';
        resp.AppendArray(&resp, (const char *[]){element.data, "\"", NULL});
        element.Destruct(&element);
    }

    if(controls) {
        if(new_content) resp.AppendString(&resp, ",");
        resp.AppendString(&resp, "\"replace_elements\": {");
        for(int i = 0; controls[i] != NULL; i++) {
            if(!controls[i])
                break;

            String element = controls[i]->Construct(controls[i], 0, 1);
            String__ReplaceChar(&element, '"', "'");

            element.data[element.idx] = '\0';
            resp.AppendArray(&resp, (const char *[]){"\"", controls[i]->ID, "\": \"", element.data, "\"", NULL});
            element.Destruct(&element);

            if(controls[i + 1])
                resp.AppendString(&resp, ",");
        }
        resp.AppendString(&resp, "}");
    }

    if(style) {
        if(controls) resp.AppendString(&resp, ",");
        resp.AppendString(&resp, "\"update_styles\": {");
        for(int i = 0; style[i] != NULL; i++) {
            resp.AppendArray(&resp, (const char *[]){"\"", (style[i]->Selector ? "." : NULL), NULL});
            resp.AppendArray(&resp, (const char *[]){style[i]->Class, "\": {", NULL});
            for(int c = 0; style[i]->Data[c] != NULL; c++) {
                String buff = NewString(style[i]->Data[c]);
                Array a = NewArray(NULL);
                a.Merge(&a, (void **)buff.Split(&buff, ":"));
                if(a.idx < 2)
                    break;

                buff.Set(&buff, a.arr[1]);
                buff.TrimAt(&buff, 0);
                resp.AppendArray(&resp, (const char *[]){"\"", a.arr[0], "\": \"", buff.data, "\"", NULL});
                if(style[i]->Data[c + 1])
                    resp.AppendString(&resp, ",");

                buff.Destruct(&buff);
                a.Destruct(&a);
            }
            
            resp.AppendString(&resp, "}");
            if(style[i + 1])
                resp.AppendString(&resp, ",");
        }
        resp.AppendString(&resp, "}");
    }

    resp.AppendString(&resp, "}");
    resp.data[resp.idx] = '\0';
    printf("%s\n", resp.data);
    SendResponse(server, req->Socket, OK, DefaultHeaders, ((Map){}), resp.data);
    resp.Destruct(&resp);
}

void SendSuccessNULL(cWS *server, cWR *req) {
    SendResponse(server, req->Socket, OK, DefaultHeaders, ((Map){0}), "fetched");
}

typedef struct Painter {
    Control     *Control;
    CSS         *Style;
    int         Paint;
    int         Action; // 1 = click, 2 = key
    int         Key;
    struct {
        int x, y;
    } Position, Mouse;
} Painter;

typedef Array ControlArray;
ControlArray Controls;
Painter p;

int MOVE_PANEL = 0;

Control *SetupTableRow(const char *text) {
    String r = NewString(text);
    r.data[r.idx] = '\0';
    Control *newc = CreateControl(DIV_TAG, "table_row", NULL, r.data, NULL);
    
    free(newc->SubControls);
    newc->SubControls = NULL;
    free(newc->CSS);
    newc->CSS = NULL;

    r.Destruct(&r);
    return newc;
}

Array AllControls(WebRoute *route) {
    Array n = NewArray(NULL);
    n.Append(&n, SetupTableRow("Control Tag"));
    n.Append(&n, SetupTableRow("ID"));
    n.Append(&n, SetupTableRow("Class"));
    n.Append(&n, SetupTableRow("Text"));

    if(((Array *)route->Args[1])->idx > 0) {
        for(int i = 0; ((Array *)route->Args[1])->idx; i++) {
            if(!((Array *)route->Args[1])->arr[i])
                break;

                
            n.Append(&n, SetupTableRow(FindTag((Control *)((Array *)route->Args[1])->arr[i])));
            n.Append(&n, SetupTableRow(((Control *)((Array *)route->Args[1])->arr[i])->Class));
            n.Append(&n, SetupTableRow(((Control *)((Array *)route->Args[1])->arr[i])->ID));
            n.Append(&n, SetupTableRow(((Control *)((Array *)route->Args[1])->arr[i])->Text));
        }
        
        return n;
    } else {
        n.Append(&n, SetupTableRow("NULL"));
        n.Append(&n, SetupTableRow("NULL"));
        n.Append(&n, SetupTableRow("NULL"));
        n.Append(&n, SetupTableRow("NULL"));
    }

    return n;
}

void UpdatePainter(Painter *p) {
    if(!p)
        return;

    if(!p->Mouse.x || !p->Mouse.y) {
        printf("[ - ] Error, Corrupted Event....!\n");
        return;
    }

    printf("Action: %s | X: %d | Y: %d\n", p->Action == 1 ? "Click" : "Keypress", p->Mouse.x, p->Mouse.y);
    if(!p->Paint && p->Action == 1) {
        p->Paint = 1;
    } else if(p->Paint && p->Action == 1) {
        // Update Control
    }

    printf("Updating....\n");
}

void test_page(cWS *server, cWR *req, WebRoute *route) {
    Painter p = *(Painter *)route->Args[0];
    if(req->RequestType.Is(&req->RequestType, "POST")) {
        p.Action = (!strcmp(((jKey *)req->Event.arr[1])->value, "click") ? 1 : (!strcmp(((jKey *)req->Event.arr[1])->value, "keydown") ? 2 : 0));
        p.Mouse.x = atoi(((jKey *)req->Event.arr[7])->value);
        p.Mouse.y = atoi(((jKey *)req->Event.arr[8])->value);

        if(req->Event.idx > 20 && isClicked(req->Event, "ws_form")) {
            char *new_ctrl_tag = FindKey(&req->Event, "new_ctrl_tag_input");
            char *new_ctrl_class = FindKey(&req->Event, "new_ctrl_class_input");
            char *new_ctrl_id = FindKey(&req->Event, "new_ctrl_id_input");
            char *new_ctrl_text = FindKey(&req->Event, "new_ctrl_text_input");

            printf(
                "%s | %s | %s | %s\n", 
                decode_input_symbols(new_ctrl_tag), 
                decode_input_symbols(new_ctrl_class), 
                decode_input_symbols(new_ctrl_id), 
                decode_input_symbols(new_ctrl_text)
            );

            Control *newc = CreateControl(FindTagType(new_ctrl_tag), !strcmp(new_ctrl_class, "null") ? NULL : new_ctrl_class, new_ctrl_id, new_ctrl_text, NULL);
            free(newc->SubControls);
            newc->SubControls = NULL;
            free(newc->CSS);
            newc->CSS = NULL;

            ((Painter *)route->Args[0])->Control = newc;
            ((Array *)route->Args[1])->Append((Array *)route->Args[1], newc);

            // SendSuccessNULL(server, req);
            ((Array *)route->Args[1])->arr[((Array *)route->Args[1])->idx] = NULL;

            Control *temp = stack_to_heap(Body);

            Array controls = AllControls(route);
            if(controls.idx > 0) {
                ((Control *)((Control *)temp->SubControls[0])->SubControls[8])->SubControls = controls.arr;
                ((Control *)((Control *)temp->SubControls[0])->SubControls[8])->SubControlCount = controls.idx;
            }
            controls.arr[controls.idx] = NULL;

            if(((Array *)route->Args[1])->idx > 0) {
                for(int i = 0; i < ((Array *)route->Args[1])->idx; i++)
                    temp->Append(temp, ((Array *)route->Args[1])->arr[i]);
            }
            
            temp->SubControls[temp->SubControlCount] = NULL;

            UpdateUI(server, req, temp, NULL, NULL);
            return;
        } else if(isKeyPressed(req->Event, "q", "new_ctrl_class_input")) {
            SendSuccessNULL(server, req);
            return;
        } else if(isClicked(req->Event, "move_panel") && !*(int *)route->Args[2]) {
            *(int *)route->Args[2] = 1;
        } else if(*(int *)route->Args[2]) {
            CSS *Pos = CreateCSS(MiniPanelStyle.Class, 1, (char **)MiniPanelStyle.Data);
            AppendDesign(Pos, CreateString((const char *[]){"margin-top: ", ((jKey *)req->Event.arr[7])->value, "px", NULL}));
            AppendDesign(Pos, CreateString((const char *[]){"margin-left: ", ((jKey *)req->Event.arr[8])->value, "px", NULL}));
            UpdateUI(server, req, NULL, NULL, (CSS *[]){Pos, NULL});

            *(int *)route->Args[2] = 0;
            return;
        }

        UpdatePainter(&p);
        SendSuccessNULL(server, req);
        return;
    }

    Control *temp = stack_to_heap(Body);
    temp->SubControls[temp->SubControlCount] = NULL;

    Array controls = AllControls(route);
    if(controls.idx > 0) {
        ((Control *)((Control *)temp->SubControls[0])->SubControls[8])->SubControls = controls.arr;
        ((Control *)((Control *)temp->SubControls[0])->SubControls[8])->SubControlCount = controls.idx;
    }
    controls.arr[controls.idx] = NULL;

    if(((Array *)route->Args[1])->idx > 0) {
        for(int i = 0; i < ((Array *)route->Args[1])->idx; i++)
            temp->Append(temp, ((Array *)route->Args[1])->arr[i]);
    }

    /* Send Default Template UI */
    char *template = ConstructTemplate(
        (Control *[]){&Header, temp, NULL}, 
        (CSS *[]){
            &BodyCSS, 
            &MiniPanelStyle, 
            &NewCtrlBtn, 
            &DisplayInRow, 
            &ControlTable, 
            &NewCtrlLabel, 
            &NewCtrlTxtbox, 
            &InvokeForm,
            NULL
        }, 
        1, 0, 0, 0, 1, 0
    );

    if(!template)
    {
        printf("[ - ] Error, Unable to construct template...!\n");
        SendResponse(server, req->Socket, OK, DefaultHeaders, ((Map){0}), "Error");
        free(template);
        return;
    }

    SendResponse(server, req->Socket, OK, DefaultHeaders, ((Map){0}), template);
    free(template);

    // for(int i = 0; i < temp->SubControlCount; i++) {
    //     ((Control *)temp->SubControls[i])->Destruct(temp->SubControls[i], 1, 1);
    // }
    if(temp->SubControlCount)
        DestructControls(temp);
}

int main() {
    p = (Painter){ .Mouse = {}, .Position = {} };
    Controls = NewArray(NULL);

    cWS *server = StartWebServer(NewString(""), 80, 0);
    if(!server)
    {
        printf("[ - ] Error, Unable to start web server....");
        return 1;
    }

    WebRoute *routes[] = {
        &(WebRoute){ .Name = "index", .Path = "/", .Handler = test_page, .Args = (void *[]){&p, &Controls, &MOVE_PANEL, NULL} },
        NULL
    };

    EnableLiveHandler(server);
    server->AddRoutes(server, routes);
    server->Run(server, 999, 0);
    server->Destruct(server);
    return 0;
}