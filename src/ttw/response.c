#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdarg.h>

#include "ttw/ttw.h"
#define MAX_OUTBUFFER_SIZE 1024 // 1K of headerbuffer


void ttw_response_send(const char *resp_code, const char* content_type, s_ttw_http_request *request, const char *body) {
	char header_buffer[MAX_OUTBUFFER_SIZE];
	int body_len = strlen(body);

	// build http response
	sprintf(header_buffer, "HTTP/1.0 %s\r\n", resp_code);

	// append headers
	sprintf(&header_buffer[strlen(header_buffer)], "Server: %s\r\n", SERVER_NAME);
	sprintf(&header_buffer[strlen(header_buffer)], "Content-Type: %s\r\n", content_type);
	sprintf(&header_buffer[strlen(header_buffer)], "Content-Length: %d\r\n", body_len);
		
	// append extra crlf to indicate start of body
	strcat(header_buffer, "\r\n");
	
	write(request->socket_fd, header_buffer, strlen(header_buffer));
	write(request->socket_fd, body, body_len);

	return;
}

void ttw_response_printf(const char *resp_code, const char* content_type, s_ttw_http_request *request, const char* format, ...)
{
	int maxlen = 4096;
	va_list fmtargs;
	char buffer[maxlen];
	memset(buffer, 0, maxlen);

	va_start(fmtargs, format);
	vsnprintf(buffer, maxlen - 1, format, fmtargs);
	va_end(fmtargs);

	ttw_response_send(resp_code, content_type, request, buffer);	
}
