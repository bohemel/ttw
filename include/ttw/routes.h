#ifndef _ROUTES_H_
#define _ROUTES_H_

#include "ttw/ttw.h"

typedef struct ttw_route {
	char *request_string;
	enum RouteType {RT_STATIC, RT_DYNAMIC} type;
	char *content_type;
	char *static_content_source;
	char *static_content;
	void (*callback)(s_ttw_http_request*);
} s_ttw_route;

void ttw_routes_init(s_ttw_route*, int);
int ttw_routes_match(s_ttw_http_request*, s_ttw_route**);
int ttw_routes_load_static_file(const char*, char**);

#endif
