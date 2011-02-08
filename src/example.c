#include <stdio.h>

#include "ttw/ttw.h"

void dynamic_test(s_ttw_http_request*);

void dynamic_test(s_ttw_http_request* request) {
	char value[100];
	if(ttw_request_get_param(request, "message", value))
		ttw_response_send("200 OK", "text/plain", request, value);
	else
		ttw_response_send("200 OK", "text/plain", request, "Echo");
}

int main(int argc, char **argv) {
	s_ttw_route routes[] = {
		{"/", 		RT_STATIC, "text/html", 	"resources/index.html", NULL, NULL}, 
		{"/api/echo", 	RT_DYNAMIC,"application/json", 	NULL, NULL, &dynamic_test}
	};
	ttw_routes_init(&routes[0], 2);
	ttw_init_and_start();
}
