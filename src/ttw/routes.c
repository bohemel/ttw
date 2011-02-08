#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "ttw/ttw.h";

int __NO_OF_ROUTES = 0;
s_ttw_route* __LOADED_ROUTES;

void ttw_routes_init(s_ttw_route* in_routes, int r_size) {
	__NO_OF_ROUTES = r_size;
	__LOADED_ROUTES = in_routes;
	for(int i = 0; i < __NO_OF_ROUTES; i++) {
		if(__LOADED_ROUTES[i].type == RT_STATIC) {
			if(ttw_routes_load_static_file(__LOADED_ROUTES[i].static_content_source, &__LOADED_ROUTES[i].static_content) < 0)
				ttw_fatal("Failed loading file, exiting");
		}
	}	

}

int ttw_routes_load_static_file(const char *filename, char **result) { 

	ttw_log(TTW_LOG_DEBUG, "Loading file %s into memory", filename);

	int size = 0;
	FILE *f = fopen(filename, "rb");
	if (f == NULL) { 
		*result = NULL;
		return -1; // -1 means file opening fail 
	} 
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	*result = (char *)malloc(size+1);
	if (size != fread(*result, sizeof(char), size, f)) 
	{ 
		free(*result);
		return -2; // -2 means file reading fail 
	} 
	fclose(f);
	(*result)[size] = 0;
	return size;
}

int ttw_routes_match(s_ttw_http_request* request, s_ttw_route **active_route) {
	for(int i = 0; i < __NO_OF_ROUTES; i++) {
		ttw_log(TTW_LOG_DEBUG, "Comparing \"%s\" with \"%s\"", request->path, __LOADED_ROUTES[i].request_string);
		if(strcmp(request->path, __LOADED_ROUTES[i].request_string) == 0) {
			*active_route = &__LOADED_ROUTES[i];
			return 1;
		}
	}
	return 0;
}


