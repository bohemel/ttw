#ifndef _RESPONSE_H_
#define _RESPONSE_H_

void ttw_response_send(const char*, const char*, s_ttw_http_request*, const char*);
void ttw_response_printf(const char *resp_code, const char* content_type, s_ttw_http_request *request, const char* format, ...);

#endif
