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
			&(Control){ .Tag = INPUT_TAG, .Class = "txtbox", .Type = "text", .ID = "new_control_type", .Name = "new_control_type", .Data = "placeholder=\"Type\"" },
			&(Control){ .Tag = INPUT_TAG, .Class = "txtbox", .Type = "text", .ID = "new_control_text", .Name = "new_control_text", .Data = "placeholder=\"Text\"" },
			&(Control){ .Tag = INPUT_TAG, .Class = "txtbox", .Type = "text", .ID = "new_control_class", .Name = "new_control_class", .Data = "placeholder=\"Class\"" },
			&(Control){ .Tag = INPUT_TAG, .Class = "txtbox", .Type = "text", .ID = "new_control_css", .Name = "new_control_css", .Data = "placeholder=\"CSS\"" },
			&(Control){ .Tag = INPUT_TAG, .Class = "txtbox", .Type = "text", .ID = "new_control_href", .Name = "new_control_href", .Data = "placeholder=\"href\"" },
			&(Control){ .Tag = INPUT_TAG, .Class = "txtbox", .Type = "text", .ID = "new_control_data", .Name = "new_control_data", .Data = "placeholder=\"Data\"" },
			&(Control){ .Tag = INPUT_TAG, .Class = "txtbox", .Type = "submit", .ID = "add_new_control", .Name = "add_new_control", .Data = "value=\"Add Control\"" },
			&(Control){ .Tag = P_TAG, .CSS = (char *[]){"margin-left: 10px", NULL} },
			NULL
		}},
		NULL
	}},
	NULL
}};

void testp(cWS *server, cWR *req, WebRoute *route, int sock) {
	printf("%ld", UIManager->StyleCount);
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

void send_index_response(cWS *server, int sock, char *t) {
	if(!t)
		((Control *)((Control *)((Control *)BodyCHT.SubControls[0])->SubControls[1])->SubControls[9])->Text = t;

	char *template = ConstructTemplate((Control *[]){&HeaderCHT, &BodyCHT, NULL}, IndexCSS);
	if(!template) {
		printf("[ - ] Error, Unable to construct template....!\n");
		SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), ((Map){0}), "Error\n\n");
	}

	SendResponse(server, sock, OK, DefaultHeaders, ((Map){0}), ((Map){0}), template);
}

void indexp(cWS *server, cWR *req, WebRoute *route, int sock) {
	char *newc_tag = decode_input_symbols(FindKey(&req->Queries, "new_control_tag"));
	char *newc_id = decode_input_symbols(FindKey(&req->Queries, "new_control_id"));
	char *newc_type = decode_input_symbols(FindKey(&req->Queries, "new_control_type"));
	char *newc_text = decode_input_symbols(FindKey(&req->Queries, "new_control_text"));
	char *newc_class = decode_input_symbols(FindKey(&req->Queries, "new_control_class"));
	char *newc_css = decode_input_symbols(FindKey(&req->Queries, "new_control_css"));
	char *newc_href = decode_input_symbols(FindKey(&req->Queries, "new_control_href"));
	char *newc_data = decode_input_symbols(FindKey(&req->Queries, "new_control_data"));

	printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s", newc_tag, newc_id, newc_type, newc_text, newc_css, newc_class, newc_href, newc_data);

	if(req->RequestType.Is(&req->RequestType, "POST") && FindKey(&req->Queries, "add_new_control")) {
		if(!newc_tag) {
			send_index_response(server, sock, "Invalid Tag!");
			return;
		}

		Control *newc = CreateControl(FindTagType(newc_tag), newc_class, newc_id, newc_text, NULL);
		if(!newc || newc->Tag == SCRIPT_TAG) {
			send_index_response(server, sock, "A tag is needed to add a control!");
			return;
		}
		
		String leftover = NewString(NULL);
		if(newc_id != NULL) {
			newc->Name = strdup(newc_id);
		}

		if(newc_type != NULL)
			newc->Type = strdup(newc_type);

		if(newc_class != NULL && newc_css != NULL) {
			if(strstr(newc_css, ",")) {
				printf("here 3\n");
				String rcss = NewString(newc_css);
				Array csss = NewArray(NULL);
				csss.Append(&csss, (void **)rcss.Split(&rcss, ","));
				
				csss.arr[csss.idx] = NULL;
				CSS *c = CreateCSS(newc_class, 1, (const char **)csss.arr);

				rcss.Destruct(&rcss);
				csss.Destruct(&csss);
				int check_css = UIManager->AppendCSS(UIManager, c);
				if(!check_css)
					printf("[ - ] Unable to add CSS....!");
			} else {
				printf("here 2\n");
				CSS *c = CreateCSS(newc_class, 1, (const char *[]){newc_css, NULL});
				if(newc->CSS != NULL)
					free(newc->CSS);

				newc->CSS = NULL;
				UIManager->AppendCSS(UIManager, c);
			}
		} else if(newc_css != NULL) {
			if(strstr(newc_css, ",")) {
				printf("here 4\n");
				String n = NewString(newc_css);
				newc->CSS = n.Split(&n, ",");
				while(newc->CSS[newc->CSSCount] != NULL)
						newc->CSSCount++;
			} else {
				printf("here\n");
				newc->AppendCSS(newc, newc_css);
			}
		}

		if(newc_href != NULL) 
			newc->href = strdup(newc_href);

		if(newc_data != NULL)
			newc->Data = CreateString((char *[]){newc_data, leftover.data, NULL});

		if(newc->SubControls != NULL)
			free(newc->SubControls);

		newc->SubControls = NULL;
		int add_chk = UIManager->Append(UIManager, newc);
		if(!add_chk) {
			free(newc);
			leftover.Destruct(&leftover);
			send_index_response(server, sock, "Unable to add control!");
			return;
		}

		leftover.Destruct(&leftover);
		send_index_response(server, sock, "Control Added!");
		return;
	}

	
	send_index_response(server, sock, NULL);
}

int main() {
    /* Initialize Webserver */
	UIManager = InitManager("/");
    cWS *server = StartWebServer(NewString(""), 80, 0);
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