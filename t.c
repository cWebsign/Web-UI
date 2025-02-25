#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "t.h"

WebUI *UIManager;

CSS NewControlForm = (CSS){ .Class = "create_control_form", .Selector = 1, .Data = (char *[]){
	"border: 2px solid black",
	"width: 400px",
	"margin-bottom: 20px",
	NULL
}};

CSS NewControlTitle = (CSS){ .Class = "new_control_title", .Selector = 1, .Data = (char *[]){
	"margin-left: 10px",
	NULL
}};

CSS Txtbox = (CSS){ .Class = "txtbox", .Selector = 1, .Data = (char *[]){
	"margin-top: 5px", 
	"margin-left: 10px", 
	"font-size: 17",
	"width: 380px",
	NULL
}};

CSS *IndexCSS[] = (CSS *[]){
	&NewControlForm, &NewControlTitle, &Txtbox, 
	NULL
};

Control HeaderCHT = (Control){ .Tag = HEAD_TAG, .SubControlCount = 1, .SubControls = (void *[]){
	&(Control){ .Tag = TITLE_TAG, .Text = "WebUI Manager" },
	NULL
}};

Control BodyCHT = (Control){ .Tag = BODY_TAG, .SubControlCount = 1, .SubControls = (void *[]){
	&(Control){ .Tag = DIV_TAG, .Class = "create_control_form", .SubControlCount = 1, .SubControls = (void *[]){
		&(Control){ .Tag = H1_TAG, .Class = "new_control_title", .Text = "Create New Control" },
		&(Control){ .Tag = FORM_TAG, .Data = "method=\"post\"", .SubControlCount = 8, .SubControls = (void *[]){
			&(Control){ .Tag = INPUT_TAG, .Class = "txtbox", .Type = "text", .ID = "new_control_tag", .Name = "new_control_tag", .Data = "placeholder=\"Tag\"" },
			&(Control){ .Tag = INPUT_TAG, .Class = "txtbox", .Type = "text", .ID = "new_control_id", .Name = "new_control_id", .Data = "placeholder=\"ID\"" },
			&(Control){ .Tag = INPUT_TAG, .Class = "txtbox", .Type = "text", .ID = "new_control_type", .Name = "new_control_type", .Data = "placeholder=\"Name\"" },
			&(Control){ .Tag = INPUT_TAG, .Class = "txtbox", .Type = "text", .ID = "new_control_text", .Name = "new_control_text", .Data = "placeholder=\"Text\"" },
			&(Control){ .Tag = INPUT_TAG, .Class = "txtbox", .Type = "text", .ID = "new_control_class", .Name = "new_control_class", .Data = "placeholder=\"Class\"" },
			&(Control){ .Tag = INPUT_TAG, .Class = "txtbox", .Type = "text", .ID = "new_control_css", .Name = "new_control_css", .Data = "placeholder=\"CSS\"" },
			&(Control){ .Tag = INPUT_TAG, .Class = "txtbox", .Type = "text", .ID = "new_control_href", .Name = "new_control_href", .Data = "placeholder=\"href\"" },
			&(Control){ .Tag = INPUT_TAG, .Class = "txtbox", .Type = "text", .ID = "new_control_data", .Name = "new_control_data", .Data = "placeholder=\"Data\"" },
			&(Control){ .Tag = INPUT_TAG, .Class = "txtbox", .Type = "submit", .ID = "add_new_control", .Name = "add_new_control", .Data = "value=\"Add Control\"" },
			&(Control){ .Tag = P_TAG, .CSS = (char *[]){"margin-left: 10px", NULL}, .Text = "Test" },
			NULL
		}},
		NULL
	}},
	NULL
}};

void testp(cWS *server, cWR *req, WebRoute *route, int sock) {
	if(UIManager->ControlCount > 0) {
		char *template = ConstructTemplate(UIManager->Controls, UIManager->Style);
		if(!template) {
			SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), ((Map){0}), "Error, Unable to construct template!");
			return;
		}

		SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), ((Map){0}), template);
		return;
	}

	SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), ((Map){0}), "There is no controls to construct!");
}

void indexp(cWS *server, cWR *req, WebRoute *route, int sock) {
	char *newc_tag = FindKey(&req->Queries, "new_control_tag");
	char *newc_id = FindKey(&req->Queries, "new_control_id");
	char *newc_type = FindKey(&req->Queries, "new_control_type");
	char *newc_text = FindKey(&req->Queries, "new_control_text");
	char *newc_css = FindKey(&req->Queries, "new_control_css");
	char *newc_class = FindKey(&req->Queries, "new_control_class");
	char *newc_href = FindKey(&req->Queries, "new_control_href");
	char *newc_data = FindKey(&req->Queries, "new_control_data");

	if(req->RequestType.Is(&req->RequestType, "POST") && FindKey(&req->Queries, "add_new_control")) {
		if(!newc_tag) {
			((Control *)((Control *)((Control *)BodyCHT.SubControls[0])->SubControls[1])->SubControls[7])->Text = "A tag is needed to add a control!";
			return;
		}

		Control *newc = CreateControl(FindTagType(newc_tag), NULL, newc_id, newc_text, NULL);
		
		String leftover = NewString(NULL);
		if(newc_type != NULL)
			newc->Type = strdup(newc_type);
		
		if(newc_text != NULL)
			newc->Text = strdup(newc_text);

		if(newc_class != NULL && newc_css != NULL) {
			String rcss = NewString(newc_css);
			Array csss = NewArray(NULL);
			csss.Append(&csss, (void **)rcss.Split(&rcss, ","));
			
			csss.arr[csss.idx] = NULL;
			CSS *c = CreateCSS(newc_class, 1, (const char **)csss.arr);

			rcss.Destruct(&rcss);
			csss.Destruct(&csss);
			int check_css = AppendStyle(route, c);
			if(!check_css)
				printf("[ - ] Unable to add CSS....!");

		} else if(newc_css != NULL) {
			leftover.AppendArray(&leftover, (const char *[]){"style=\"", newc_css, "\"", NULL});
		}

		if(newc_href != NULL) 
			newc->href = strdup(newc_href);

		if(newc_data != NULL)
			newc->Data = strdup(newc_data);

		int add_chk = UIManager->Append(UIManager, newc);
		if(!add_chk)
			((Control *)((Control *)((Control *)BodyCHT.SubControls[0])->SubControls[1])->SubControls[7])->Text = "Unable to add control!";
	}

	char *template = ConstructTemplate((Control *[]){&HeaderCHT, &BodyCHT, NULL}, IndexCSS);
	if(!template) {
		printf("[ - ] Error, Unable to construct template....!\n");
		SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), ((Map){0}), "Error\n\n");
	}

	SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), ((Map){0}), template);
}

int main() {
    /* Initialize Webserver */
	UIManager = InitManager("/");
    cWS *server = StartWebServer(NewString(""), 60, 0);
    if(!server)
        return 1;

    /* Add a route */
    server->AddRoutes(server, (WebRoute *[]){
        &(WebRoute){ .Name = "index", .Path = "/", .Handler = indexp, .Args = (void *[]){server, NULL} },
		&(WebRoute){ .Name = "test", .Path = "/test", .Handler = testp },
		NULL
    });

    /* Run the web server in background */
    server->Run(server, 999, 0);

    char BUFF[10];
    fgets(BUFF, 10, stdin);
    return 0;
}