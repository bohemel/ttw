#ifndef _TTW_H_
#define _TTW_H_

#include "ttw/request.h"
#include "ttw/response.h"
#include "ttw/routes.h"

#define SERVER_NAME "TTW (tiny tiny web)"
#define SERVER_VERSION "v1"

#define TTW_LOG_ERROR		40
#define TTW_LOG_WARNING 	30
#define TTW_LOG_INFO		20
#define TTW_LOG_DEBUG		10

void ttw_log(const int, const char*, ... );
void ttw_fatal(const char*);

void ttw_dispatch_request(int);
void ttw_destroy_request(s_ttw_http_request*);
void ttw_init_and_start();

#endif

