#include "parse.h"

/**
* Given a char buffer returns the parsed request headers
*/



Request** parse(char *buffer, int size, int socketFd,int* request_num) {
	//1. divided to multiple requests and stored to substring
	//2. set_parsing_options and yyparse, using loop store to **requests
	//3. return 
	int i = 0;
	int temp = 0;
    //Valid End State
	Request *request = (Request *) malloc(sizeof(Request));
	//restore the information of each request
	char** substring = (char**)malloc(sizeof(char*)*8192);
    char** address = (char**)malloc(sizeof(char*)*8192);
    int* strlength = (int*)malloc(sizeof(int)*8192);
	for(i=0;i<100;i++){
		substring[i] = (char*)malloc(sizeof(char)*8192);
        address[i] = (char*)malloc(sizeof(char)*8192);
	}
	
	
	request->header_count=0;
	//TODO You will need to handle resizing this in parser.y
	
	// request->headers = (Request_header *) malloc(sizeof(Request_header)*1); //storage overflow
	//TODO I need to parse like this: 1.get substring 2.transform to request 3.store request to requests
	address[0] = buffer;

	while(strstr(address[*request_num],"\r\n\r\n")){
		*request_num = *request_num + 1;
		address[*request_num] = strstr(address[*request_num - 1],"\r\n\r\n");
		address[*request_num] = address[*request_num] + 4;

	}

	strlength[0] = address[1]-address[0];
	strncpy(substring[0],address[0],strlength[0]);

	for(temp = 1;temp < *request_num;temp++){
		strlength[temp] = address[temp + 1]-address[temp];
		strncpy(substring[temp],address[temp],strlength[temp]);
	}

	Request** requests = (Request**)malloc(sizeof(Request*)*4000);
	for(i=0;i<*request_num;i++){	
		requests[i] = (Request*)malloc(sizeof(Request)*1000);
	}

	for(i = 0;i < *request_num; i++){
		set_parsing_options(substring[i], strlength[i], request);
		yylex_destroy();
		if (yyparse() == SUCCESS) {
			strcpy(requests[i]->http_method, request->http_method);
			strcpy(requests[i]->http_uri, request->http_uri);
			strcpy(requests[i]->http_version, request->http_version);
		}
		else{
			requests[i] = NULL;
            free(requests[i]);
		}
	}


	return requests;

    //TODO Handle Malformed Requests
	yylex_destroy();
    printf("Parsing Failed\n");
	return NULL;
}


