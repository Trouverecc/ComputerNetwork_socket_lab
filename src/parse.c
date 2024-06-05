#include "parse.h"

/**
* Given a char buffer returns the parsed request headers
*/
Request * parse(char *buffer, int size, int socketFd) {
	yylex_destroy();
    //Differant states in the state machine
	enum {
		STATE_START = 0, STATE_CR, STATE_CRLF, STATE_CRLFCR, STATE_CRLFCRLF
	};

	int i = 0, state;
	size_t offset = 0;
	char ch;
	char buf[8192];
	memset(buf, 0, 8192);

	state = STATE_START;
	while (state != STATE_CRLFCRLF) {
		char expected = 0;

		if (i == size)
			break;

		ch = buffer[i++];
		buf[offset++] = ch;

		switch (state) {
		case STATE_START:
		case STATE_CRLF:
			expected = '\r';
			break;
		case STATE_CR:
		case STATE_CRLFCR:
			expected = '\n';
			break;
		default:
			state = STATE_START;
			continue;
		}

		if (ch == expected)
			state++;
		else
			state = STATE_START;

	}

    //Valid End State
	if (state == STATE_CRLFCRLF) {
           	Request *request = (Request *) malloc(sizeof(Request) * 1);
        	request->header_count=0;
        	//TODO You will need to handle resizing this in parser.y
        	request->headers = (Request_header *) malloc(sizeof(Request_header)*100);
			set_parsing_options(buf, i, request); 
			if(yyparse() == SUCCESS) 
				return request;
	}
    //TODO Handle Malformed Requests
    printf("Parsing Failed\n");
	return NULL;
}


// #include "parse.h"

// /**
// * Given a char buffer returns the parsed request headers
// */
// Request * parse(char *buffer, int size, int socketFd) {
//   //Differant states in the state machine
// 	// yylex_destroy();
// 	enum {
// 		STATE_START = 0, STATE_CR, STATE_CRLF, STATE_CRLFCR, STATE_CRLFCRLF
// 	};

// 	int i = 0, state;
// 	size_t offset = 0;
// 	char ch;
// 	char buf[8192];
// 	memset(buf, 0, 8192);

// 	state = STATE_START;
// 	while (state != STATE_CRLFCRLF) {
// 		char expected = 0;

// 		if (i == size)
// 			break;

// 		ch = buffer[i++];
// 		buf[offset++] = ch;

// 		switch (state) {
// 		case STATE_START:
// 		case STATE_CRLF:
// 			expected = '\r';
// 			break;
// 		case STATE_CR:
// 		case STATE_CRLFCR:
// 			expected = '\n';
// 			break;
// 		default:
// 			state = STATE_START;
// 			continue;
// 		}

// 		if (ch == expected)
// 			state++;
// 		else
// 			state = STATE_START;
// 		// printf("State: %d, Char: %c\n", state, ch); // 调试输出

// 	}

//     //Valid End State
// 	if (state == STATE_CRLFCRLF) {
// 		Request *request = (Request *) malloc(sizeof(Request));
//         request->header_count=0;
//         //TODO You will need to handle resizing this in parser.y
//         //request->headers = (Request_header *) malloc(sizeof(Request_header)*1);//原
// 		//request->headers = (Request_header *) malloc(sizeof(Request_header)*100);//try1-1
// 		request->headers = NULL;
// 		set_parsing_options(buf, i, request);

// 		if (yyparse() == SUCCESS) {
//             return request;
// 		}
// 		 else {
//             fprintf(stderr, "Parsing Failed in yyparse()\n");
//             free(request);
//         }
// 	}
//     //TODO Handle Malformed Requests
//     // yylex_destroy();
// 	// printf("Parsing Failed\n");
// 	else {
//         fprintf(stderr, "Parsing Failed due to incomplete request\n");
// 		// printf("Buffer content: %s\n", buf); // 调试输出缓冲区内容
//     }
// 	return NULL;
// }

