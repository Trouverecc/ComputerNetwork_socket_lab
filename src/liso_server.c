/******************************************************************************
* echo_server.c                                                               *
*                                                                             *
* Description: This file contains the C source code for an echo server.  The  *
*              server runs on a hard-coded port and simply write back anything*
*              sent to it by connected clients.  It does not support          *
*              concurrent clients.                                            *
*                                                                             *
* Authors: Athula Balachandran <abalacha@cs.cmu.edu>,                         *
*          Wolf Richter <wolf@cs.cmu.edu>                                     *
*                                                                             *
*******************************************************************************/

// #include <netinet/in.h>
// #include <netinet/ip.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <time.h>
// #include <sys/stat.h>
// #include "parse.h"

// #define ECHO_PORT 9999
// #define BUF_SIZE 4096

// Request* parse(char* buffer, int size, int socketFd);

// char* generate_date_header(char* buffer, size_t size, const char* prefix) {
//     time_t rawtime;
//     struct tm *info;
    
//     time(&rawtime);
//     info = gmtime(&rawtime);
//     strftime(buffer, size, "%a, %d %b %Y %H:%M:%S UTC", info);
    
//     static char header[BUF_SIZE];
//     snprintf(header, sizeof(header), "%s%s\r\n", prefix, buffer);
//     return header;
// }

// char* generate_last_modified_header(const char* filepath) {
//     struct stat filestat;
//     stat(filepath, &filestat);

//     struct tm *info = localtime(&filestat.st_mtime);
//     static char buffer[BUF_SIZE];
//     strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", info);

//     static char header[BUF_SIZE];
//     snprintf(header, sizeof(header), "Last-Modified: %s\r\n", buffer);
//     return header;
// }

// char* handle_get_request(const char* request_uri) {
//     static char response[BUF_SIZE];
//     char htmlstr[BUF_SIZE];
//     FILE *index = fopen("./static_site/index.html", "r");

//     if (!index) {
//         snprintf(response, sizeof(response), "HTTP/1.1 404 Not Found\r\n\r\n");
//         return response;
//     }

//     fread(htmlstr, BUF_SIZE, 1, index);
//     fclose(index);

//     snprintf(response, sizeof(response),
//              "HTTP/1.1 200 OK\r\n"
//              "Server: liso/1.1\r\n"
//              "%s"
//              "Content-Length: %ld\r\n"
//              "Content-Type: text/html\r\n"
//              "%s"
//              "Connection: keep-alive\r\n\r\n"
//              "%s",
//              generate_date_header(response, sizeof(response), "Date: "),
//              strlen(htmlstr),
//              generate_last_modified_header("./static_site/index.html"),
//              htmlstr);

//     return response;
// }

// char* handle_head_request() {
//     static char response[BUF_SIZE];

//     snprintf(response, sizeof(response),
//              "HTTP/1.1 200 OK\r\n"
//              "Server: Apache-Coyote/1.1\r\n"
//              "Content-Type: text/html;charset=UTF-8\r\n"
//              "%s"
//              "\r\n",
//              generate_date_header(response, sizeof(response), "Date: "));

//     return response;
// }

// char* generate_error_response(const char* status) {
//     static char response[BUF_SIZE];
//     snprintf(response, sizeof(response), "%s\r\n\r\n", status);
//     return response;
// }

// char* process(char* buf, int size) {
//     printf("Received request: %.*s\n", size, buf); // 调试信息

//     Request* request = parse(buf, size, 0);

//     if (request) {
//         printf("Parsed request: Method=%s, URI=%s, Version=%s\n",
//                request->http_method, request->http_uri, request->http_version); // 调试信息

//         if (strcmp(request->http_version, "HTTP/1.1") == 0) {
//             if (strcmp(request->http_method, "GET") == 0) {
//                 if (strcmp(request->http_uri, "/") ) {
//                     return generate_error_response("HTTP/1.1 404 Not Found");
//                 }
//                 return handle_get_request(request->http_uri);
//             } else if (strcmp(request->http_method, "HEAD") == 0) {
//                 return handle_head_request();
//             } else if (strcmp(request->http_method, "POST") == 0) {
//                 return buf;  // Handle POST request if needed
//             } else {
//                 return generate_error_response("HTTP/1.1 501 Not Implemented");
//             }
//         } else {
//             return generate_error_response("HTTP/1.1 505 HTTP Version Not Supported");
//         }
//     } else {
//         return generate_error_response("HTTP/1.1 400 Bad Request");
//     }
// }


// int close_socket(int sock) {
//     if (close(sock)) {
//         fprintf(stderr, "Failed closing socket.\n");
//         return 1;
//     }
//     return 0;
// }

// int main(int argc, char* argv[]) {
//     int sock, client_sock;
//     ssize_t readret;
//     socklen_t cli_size;
//     struct sockaddr_in addr, cli_addr;
//     char buf[BUF_SIZE];
//     char *pbuf;

//     fprintf(stdout, "----- Liso Server -----\n");
    
//     /* all networked programs must create a socket */
//     if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
//         fprintf(stderr, "Failed creating socket.\n");
//         return EXIT_FAILURE;
//     }

//     addr.sin_family = AF_INET;        // IPv4
//     addr.sin_port = htons(ECHO_PORT); // port 9999
//     addr.sin_addr.s_addr = INADDR_ANY;// ip=0.0.0.0 (本机)

//     /* servers bind sockets to ports---notify the OS they accept connections */
//     // 绑定
//     if (bind(sock, (struct sockaddr *) &addr, sizeof(addr))) {
//         close_socket(sock);
//         fprintf(stderr, "Failed binding socket.\n");
//         return EXIT_FAILURE;
//     }

//     // 监听
//     if (listen(sock, 5)) {
//         close_socket(sock);
//         fprintf(stderr, "Error listening on socket.\n");
//         return EXIT_FAILURE;
//     }

//     /* finally, loop waiting for input and then write it back */
//     while (1) {
//         cli_size = sizeof(cli_addr);
//         if ((client_sock = accept(sock, (struct sockaddr *) &cli_addr, &cli_size)) == -1) {
//             close(sock);
//             fprintf(stderr, "Error accepting connection.\n");
//             return EXIT_FAILURE;
//         }

//         readret = 0;

//         while((readret = recv(client_sock, buf, BUF_SIZE, 0)) >= 1) {
//             pbuf = process(buf, strlen(buf)); // 修正传递的 size
//             printf("server: %s\n", pbuf);
//             send(client_sock, pbuf, strlen(pbuf), 0);
//             memset(buf, 0, BUF_SIZE);
//         } 

//         if (readret == -1) {
//             close_socket(client_sock);
//             close_socket(sock);
//             fprintf(stderr, "Error reading from client socket.\n");
//             return EXIT_FAILURE;
//         }

//         if (close_socket(client_sock)) {
//             close_socket(sock);
//             fprintf(stderr, "Error closing client socket.\n");
//             return EXIT_FAILURE;
//         }
//     }

//     // close_socket(sock);

//     return EXIT_SUCCESS;
// }



#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include "parse.h"

#define ECHO_PORT 9999
#define BUF_SIZE 4096

Request* parse(char* buffer, int size, int socketFd);

// FILE * log;

char* generate_date_header(char* buffer, size_t size, const char* prefix) {
    time_t rawtime;
    struct tm *info;
    
    time(&rawtime);
    info = gmtime(&rawtime);
    strftime(buffer, size, "%a, %d %b %Y %H:%M:%S UTC", info);
    
    static char header[BUF_SIZE];
    snprintf(header, sizeof(header), "%s%s\r\n", prefix, buffer);
    return header;
}

char* generate_last_modified_header(const char* filepath) {
    struct stat filestat;
    stat(filepath, &filestat);

    struct tm *info = localtime(&filestat.st_mtime);
    static char buffer[BUF_SIZE];
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", info);

    static char header[BUF_SIZE];
    snprintf(header, sizeof(header), "Last-Modified: %s\r\n", buffer);
    return header;
}

char* handle_get_request(const char* request_uri) {
    static char response[BUF_SIZE];
    char htmlstr[BUF_SIZE];
    FILE *index = fopen("./static_site/index.html", "r");

    if (!index) {
        snprintf(response, sizeof(response), "HTTP/1.1 404 Not Found\r\n\r\n");
        return response;
    }

    fread(htmlstr, BUF_SIZE, 1, index);
    fclose(index);

    snprintf(response, sizeof(response),
             "HTTP/1.1 200 OK\r\n"
             "Server: liso/1.1\r\n"
             "%s"
             "Content-Length: %ld\r\n"
             "Content-Type: text/html\r\n"
             "%s"
             "Connection: keep-alive\r\n\r\n"
             "%s",
             generate_date_header(response, sizeof(response), "Date: "),
             strlen(htmlstr),
             generate_last_modified_header("./static_site/index.html"),
             htmlstr);

    return response;
}

char* handle_head_request() {
    static char response[BUF_SIZE];

    snprintf(response, sizeof(response),
             "HTTP/1.1 200 OK\r\n"
             "Server: Apache-Coyote/1.1\r\n"
             "Content-Type: text/html;charset=UTF-8\r\n"
             "%s"
             "\r\n",
             generate_date_header(response, sizeof(response), "Date: "));

    return response;
}

char* generate_error_response(const char* status) {
    static char response[BUF_SIZE];
    snprintf(response, sizeof(response), "%s\r\n\r\n", status);
    return response;
}

char* process(char* buf, int size) {
    printf("Received request: %.*s\n", size, buf); // 调试信息

    Request* request = parse(buf, size, 0);

    if (request) {
        printf("Parsed request: Method=%s, URI=%s, Version=%s\n",
               request->http_method, request->http_uri, request->http_version); // 调试信息

        if(strcmp(request->http_version, "HTTP/1.1") != 0){
            memset(buf, 0, BUF_SIZE);
            return generate_error_response("HTTP/1.1 505 HTTP Version Not Supported");
        }
        if (strcmp(request->http_version, "HTTP/1.1") == 0) {
            if (strcmp(request->http_method, "GET") == 0) {
                if (strcmp(request->http_uri, "/") != 0) {
                    return generate_error_response("HTTP/1.1 404 Not Found");
                }
                return handle_get_request(request->http_uri);
            } else if (strcmp(request->http_method, "HEAD") == 0) {
                return handle_head_request();
            } else if (strcmp(request->http_method, "POST") == 0) {
                return buf;  // Handle POST request if needed
            } else {
            //    fprintf(log, "%s - - \"%s %s %s\" %s %ld\n", request->headers[0].header_value, request->http_method, request->http_uri, request->http_version, "501", 50);
            //    fflush(log);
               return generate_error_response("HTTP/1.1 501 Not Implemented");
            }
        }
       
    } else {
        return generate_error_response("HTTP/1.1 400 Bad Request");
    }
}

int close_socket(int sock) {
    if (close(sock)) {
        fprintf(stderr, "Failed closing socket.\n");
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    int sock, client_sock;
    ssize_t readret;
    socklen_t cli_size;
    struct sockaddr_in addr, cli_addr;
    char buf[BUF_SIZE];
    char *pbuf;

    // log = fopen("access_log", "w+");
    fprintf(stdout, "----- Liso Server -----\n");
    
    /* all networked programs must create a socket */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Failed creating socket.\n");
        return EXIT_FAILURE;
    }

    addr.sin_family = AF_INET;        // IPv4
    addr.sin_port = htons(ECHO_PORT); // port 9999
    addr.sin_addr.s_addr = INADDR_ANY;// ip=0.0.0.0 (本机)

    /* servers bind sockets to ports---notify the OS they accept connections */
    // 绑定
    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr))) {
        close_socket(sock);
        fprintf(stderr, "Failed binding socket.\n");
        return EXIT_FAILURE;
    }

    // 监听
    if (listen(sock, 5)) {
        close_socket(sock);
        fprintf(stderr, "Error listening on socket.\n");
        return EXIT_FAILURE;
    }

    /* finally, loop waiting for input and then write it back */
    while (1) {
        cli_size = sizeof(cli_addr);
        if ((client_sock = accept(sock, (struct sockaddr *) &cli_addr, &cli_size)) == -1) {
            close(sock);
            fprintf(stderr, "Error accepting connection.\n");
            return EXIT_FAILURE;
        }

        readret = 0;

        while((readret = recv(client_sock, buf, BUF_SIZE, 0)) >= 1) {
            pbuf = process(buf, strlen(buf)); // 修正传递的 size
            printf("server: %s\n", pbuf);
            send(client_sock, pbuf, strlen(pbuf), 0);
            memset(buf, 0, BUF_SIZE);
        } 

        if (readret == -1) {
            close_socket(client_sock);
            close_socket(sock);
            fprintf(stderr, "Error reading from client socket.\n");
            return EXIT_FAILURE;
        }

        if (close_socket(client_sock)) {
            close_socket(sock);
            fprintf(stderr, "Error closing client socket.\n");
            return EXIT_FAILURE;
        }
    }

    // close_socket(sock);

    return EXIT_SUCCESS;
}

