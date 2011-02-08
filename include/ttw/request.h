#ifndef _REQUEST_H_
#define _REQUEST_H_

#define MAX_REQUEST_PATH_LENGTH  255

typedef struct ttw_http_request {
	enum HttpMethod {METHOD_GET, METHOD_POST} method;
	char path[MAX_REQUEST_PATH_LENGTH];
	char query[MAX_REQUEST_PATH_LENGTH];
	int socket_fd;
} s_ttw_http_request;

int ttw_parse_request(char*, s_ttw_http_request*);
int ttw_request_get_param(s_ttw_http_request*, const char*, char*);

#endif
