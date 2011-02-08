#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "ttw/ttw.h"

#define BUFSIZE 8096

void ttw_log(const int level, const char *format, ...) {
	
	switch(level) {
		case TTW_LOG_ERROR:
			printf("Error: ");
			break;
		case TTW_LOG_WARNING:
			printf("Warning: ");
			break;
		case TTW_LOG_INFO:
			printf("Info: ");
			break;
		case TTW_LOG_DEBUG:
			printf("Debug: ");
			break;
		default:
			printf("Unknown level: ");
			break;
	}
		
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
	printf("\n");
}

void ttw_fatal(const char *msg) {
	perror(msg);
	exit(1);
}

void ttw_dispatch_request(int socket_fd) {

	static char buffer[BUFSIZE + 1];
	static s_ttw_http_request request;

	request.socket_fd = socket_fd;

	long ret = read(socket_fd,buffer,BUFSIZE); 	// read Web request in one go
	if(ret > 0 && ret < BUFSIZE)			// return code is valid chars 
		buffer[ret]=0;				// terminate the buffer
	else buffer[0]=0;

	if(ttw_parse_request(buffer, &request) == 0) {
		ttw_log(TTW_LOG_WARNING, "Could not parse incomming request");
		ttw_log(TTW_LOG_DEBUG, "%s", buffer);
		ttw_response_send("400 Bad Request", "text/plain", &request, "Could not parse HTTP headers");
	}

	s_ttw_route *active_route;
	if(ttw_routes_match(&request, &active_route) == 0) {
		ttw_log(TTW_LOG_WARNING, "404 on %s", request.path);
		ttw_response_send("404 Not Found", "text/plain", &request, "Resource not found");
	}
	else {
		if(active_route->type == RT_STATIC) {
			ttw_log(TTW_LOG_INFO, "Found static route %s", active_route->static_content_source);
			ttw_response_send("200 OK", active_route->content_type, &request, active_route->static_content);
		}
		else if(active_route->type == RT_DYNAMIC) {
			ttw_log(TTW_LOG_INFO, "Found dynamic route %s", active_route->request_string);
			(*active_route->callback)(&request);
		}
		else {
			ttw_log(TTW_LOG_ERROR, "Route type not in defined range!");
			ttw_response_send("501 internal server error", "text/plain", &request, "Oh darn! Parser error!");
		}
	}
	ttw_destroy_request(&request);
} 

void ttw_destroy_request(s_ttw_http_request *request) {
	close(request->socket_fd);
} 

void ttw_init_and_start() {

	int sockfd, newsockfd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		ttw_fatal("ERROR opening socket");

	int portno = 8080;
	struct sockaddr_in serv_addr, cli_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		ttw_fatal("ERROR on binding");

	ttw_log(TTW_LOG_INFO, "%s now online on port %d", SERVER_NAME, portno);

	listen(sockfd,5);

	socklen_t clilen;
	clilen = sizeof(cli_addr);

	while(1) {
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
		if (newsockfd < 0) 
			ttw_fatal("ERROR on accept");
		
		// handle new connection
		ttw_dispatch_request(newsockfd);
	}

	close(sockfd);
}
 
