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

// Request** parse(char *buffer, int size, int socketFd, int* request_num);

// char* process(char*buf, int size){
//     // FILE * log;
//     // log = fopen("Log.txt", "w");
//     int i = 0;
//     char temp[100];
//     time_t Time;
//     struct tm *info;
//     Time = time(NULL);
//     info = localtime(&Time);

//     Request** request = (Request**)malloc(sizeof(Request*)*1000);
//     for(i=0;i<100;i++){
//         //each zone has one request
//         request[i] = (Request*)malloc(sizeof(Request)*1000);
//     }
//     int* request_num = (int*)malloc(sizeof(int));
//     *request_num = 0;
//     char contentlength[1000] = {0};
//     char lastmodifiedtime[1000];
//     char* buffer = (char*)malloc(sizeof(char)*8192);
//     memset(buffer,0,BUF_SIZE*2);
//     //TODO: only get the first request, how to get others
//     request = parse(buf, size, 0, request_num);
//     int k;
//     for(k = 0;k<(*request_num);k++){
//         yylex_destroy();
        
//         if(request[k]==NULL){
//             //yylex_destroy();
//             FILE* log;
//             strftime(temp,100,"%a, %d %b %Y %k:%M:%S",info);
//             strcat(temp," UTC");
//             log = fopen("./Log.txt", "a");
//             fprintf(log,"[Date: %s][client: 127.0.0.1:38636]HTTP/1.1 400 Bad request\n",temp);
//             strcpy(buf, "HTTP/1.1 400 Bad request\r\n\r\n");
//             fclose(log);
//         }
        
//         else{
//             if((strcmp(request[k]->http_version,"HTTP/1.1")) == 0){
//                 if(!strcmp(request[k]->http_method,"GET")){
//                     //TODO: I need to return the entire web and print html
//                     char htmlstr[BUF_SIZE*2];
//                     FILE *index;
//                     FILE* log;
//                     struct stat filestat;
//                     //TODO: I need to realize 404 here
//                     if(strcmp(request[k]->http_uri,"/") != 0){
//                         //Logint(2);
//                         FILE* log;
//                         strftime(temp,100,"%a, %d %b %Y %k:%M:%S",info);
//                         strcat(temp," UTC");
//                         log = fopen("./Log.txt", "a");
//                         fprintf(log,"[Date: %s][client: 127.0.0.1:38636]HTTP/1.1 404 Not Implemented\n",temp);
//                         strcpy(buf, "HTTP/1.1 404 Not Found\r\n\r\n");
//                         fclose(log);
//                         strcat(buffer,buf);
//                         if(k==(*request_num)-1){
//                             return buffer;
//                         }
//                         else{
//                             continue;
//                         }
//                     }

//                     index = fopen("./static_site/index.html", "r");
//                     fread(htmlstr,BUF_SIZE*2,1,index);
//                     //get ready to receive message
//                     fprintf(stderr,"%s\n",buf);
//                     memset(buf,0,BUF_SIZE*2);
//                     //TODO: I need to stract("HTTP/1.1 200 OK\r\n",recinfo)
//                     strcat(buf,"HTTP/1.1 200 OK\r\n");
//                     strcat(buf,"Server: liso/1.1\r\n");
//                     strftime(temp,100,"%a, %d %b %Y %k:%M:%S",info);
//                     strcat(temp," UTC");
//                     strcat(buf, "Date: ");
//                     if((log=fopen("./Log.txt","a+"))==NULL){
//                         printf("cannot open this file\n");
//                         exit(0);
//                     }

//                     fprintf(log,"[Date: %s][client: 127.0.0.1:38636]200 OK, responded for request GET\n",temp);
                    

//                     strcat(buf,temp);
//                     strcat(buf,"\r\n");
//                     strcat(buf,"Content-Length: ");
//                     sprintf(contentlength,"%ld",strlen(htmlstr));
//                     strcat(buf,contentlength);
//                     strcat(buf,"\r\n");
//                     strcat(buf,"Content-Type: text/html\r\n");
//                     //TODO: I need to strcat the last modified time to buf
//                     strcat(buf, "Last-Modified: ");
//                     stat(request[k]->http_uri, &filestat);

//                     time_t rawtime = filestat.st_mtime; // TODO need check
//                     struct tm *infom;
//                     infom = localtime(&rawtime);
//                     strftime(lastmodifiedtime, 100, "%a, %d %b %Y %H:%M:%S", infom);
//                     strcat(lastmodifiedtime, " GMT");

//                     fprintf(log,"[Last-Modified: %s][client: 127.0.0.1:38636]200 OK, responded for request GET\n",lastmodifiedtime);
                    
//                     strcat(buf, lastmodifiedtime);
//                     strcat(buf,"\r\n");
//                     strcat(buf,"Conection: keep-alive\r\n\r\n");
                    
//                     strcat(buf,htmlstr);
//                     fclose(log);
//                     strcat(buffer,buf);
//                     if(k==(*request_num)-1){
//                         return buffer;
//                     }
//                     else{
//                         continue;
//                     }
                    
//                 }
//                 else if(!strcmp(request[k]->http_method,"HEAD")){
//                     char htmlstr[BUF_SIZE*2];
//                     FILE *index;
//                     FILE* log;
//                     struct stat filestat;
                    
//                     if(strcmp(request[k]->http_uri,"/") != 0){
//                         strcpy(buf, "HTTP/1.1 404 Not Found\r\n\r\n");
//                         strcat(buffer,buf);
//                         if(k==(*request_num)-1){
//                             return buffer;
//                         }
//                         else{
//                             continue;
//                         }
//                     }
                
//                     index = fopen("./static_site/index.html", "r");
//                     //log = fopen("Log.txt","a+");
//                     fread(htmlstr,BUF_SIZE*2,1,index);
//                     //get ready to receive message
//                     memset(buf,0,BUF_SIZE*2*2);
//                     //TODO: I need to stract("HTTP/1.1 200 OK\r\n",recinfo)
//                     strcat(buf,"HTTP/1.1 200 OK\r\n");
//                     strcat(buf,"Server: liso/1.1\r\n");

//                     strftime(temp,100,"%a, %d %b %Y %k:%M:%S",info);
//                     strcat(temp," UTC");
//                     strcat(buf, "Date: ");
                    
//                     if((log=fopen("./Log.txt","a+"))==NULL){
//                         printf("cannot open this file\n");
//                         exit(0);
//                     }

//                     fprintf(log,"[Date: %s][client: 127.0.0.1:38636]200 OK, responded for request HEAD\n",temp);
                    

//                     strcat(buf,temp);
//                     strcat(buf,"\r\n");
//                     strcat(buf,"Content-Length: ");
//                     sprintf(contentlength,"%ld",strlen(htmlstr));
//                     strcat(buf,contentlength);
//                     strcat(buf,"\r\n");
//                     strcat(buf,"Content-Type: text/html\r\n");
                    
//                     //TODO: I need to strcat the last modified time to buf
//                     strcat(buf, "Last-Modified: ");
//                     stat(request[k]->http_uri, &filestat);

//                     time_t rawtime = filestat.st_mtime; // TODO need check
//                     struct tm *infom;
//                     infom = localtime(&rawtime);
//                     strftime(lastmodifiedtime, 100, "%a, %d %b %Y %H:%M:%S", infom);
//                     strcat(lastmodifiedtime, " GMT");

//                     fprintf(log,"[Last-Modified: %s][client: 127.0.0.1:38636]200 OK, responded for request HEAD\n",lastmodifiedtime);
                    
//                     strcat(buf, lastmodifiedtime);
//                     strcat(buf,"\r\n");
//                     strcat(buf,"Conection: keep-alive\r\n\r\n");
//                     fclose(log);
//                     strcat(buffer,buf);
//                     if(k==(*request_num)-1){
//                         return buffer;
//                     }
//                     else{
//                         continue;
//                     }
//                 }
//                 else if(!strcmp(request[k]->http_method,"POST")){
//                     strftime(temp,100,"%a, %d %b %Y %k:%M:%S",info);
//                     strcat(temp," UTC");
//                     FILE* log;
//                     log = fopen("./Log.txt", "a");
//                     fprintf(log,"[Date: %s][client: 127.0.0.1:38636]200 OK, responded for request POST\n",temp);
//                     fclose(log);
//                     strcat(buffer,buf);
//                     if(k==(*request_num)-1){
//                         return buffer;
//                     }
//                     else{
//                         continue;
//                     }
//                 }
//                 else{
//                     FILE* log;
//                     strftime(temp,100,"%a, %d %b %Y %k:%M:%S",info);
//                     strcat(temp," UTC");
//                     log = fopen("./Log.txt", "a");
//                     fprintf(log,"[Date: %s][client: 127.0.0.1:38636]HTTP/1.1 501 Not Implemented\n",temp);
//                     strcpy(buf,"HTTP/1.1 501 Not Implemented\r\n\r\n");
//                     fclose(log);
//                 }
//             }
//             else{
//                 FILE* log;
//                 strftime(temp,100,"%a, %d %b %Y %k:%M:%S",info);
//                 strcat(temp," UTC");
//                 log = fopen("./Log.txt", "a");
//                 fprintf(log,"[Date: %s][client: 127.0.0.1:38636]HTTP/1.1 505 HTTP Version not supported\n",temp);
//                 strcpy(buf, "HTTP/1.1 505 HTTP Version not supported\r\n\r\n");
//                 fclose(log);
//             }
//         }
         
//         strcat(buffer,buf);
//         if(k==(*request_num)-1){
//             return buffer;
//         }
//         else{
//             continue;
//         }
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
//     char buf[BUF_SIZE*2];
//     char *pbuf;

//     // log = fopen("access_log", "w+");
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

//         while((readret = recv(client_sock, buf, BUF_SIZE*2, 0)) >= 1) {
//             pbuf = process(buf, strlen(buf)); // 修正传递的 size
//             printf("server: %s\n", pbuf);
//             send(client_sock, pbuf, strlen(pbuf), 0);
//             memset(buf, 0, BUF_SIZE*2);
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

Request** parse(char *buffer, int size, int socketFd, int* request_num);

char* process(char*buf, int size){
    char temp[100];
    time_t Time;
    struct tm *info;
    Time = time(NULL);
    info = localtime(&Time);
	strftime(temp,100,"%a, %d %b %Y %k:%M:%S",info);
    strcat(temp, " UTC");

    Request** request = (Request**)malloc(sizeof(Request*)*100);
    
    int* request_num = (int*)malloc(sizeof(int));
    *request_num = 0;
    request = parse(buf, size, 0, request_num);

    char* buffer = (char*)malloc(sizeof(char) * BUF_SIZE * (*request_num));
    memset(buffer,0,BUF_SIZE * (*request_num));
    //TODO: only get the first request, how to get others

    for(int k = 0;k<(*request_num);k++){
        char response[BUF_SIZE] = {0};
        
        if(request[k]==NULL){

            sprintf(response, "HTTP/1.1 400 Bad request\r\n\r\n");
            FILE* log = fopen("./Log.txt", "a");
            fprintf(log, "[Date: %s][client: 127.0.0.1:38636]HTTP/1.1 400 Bad request\n", temp);
            fclose(log);
        }
        
        else {
            if (strcmp(request[k]->http_version, "HTTP/1.1") == 0) {
                if (strcmp(request[k]->http_method, "GET") == 0) {
                    char htmlstr[BUF_SIZE];
                    FILE *index = fopen("./static_site/index.html", "r");
                    
                    if (index == NULL || strcmp(request[k]->http_uri, "/") != 0) {
                        sprintf(response, "HTTP/1.1 404 Not Found\r\n\r\n");
                        FILE* log = fopen("./Log.txt", "a");
                        fprintf(log, "[Date: %s][client: 127.0.0.1:38636]HTTP/1.1 404 Not Found\n", temp);
                        fclose(log);
                    } else {
                        fread(htmlstr, BUF_SIZE, 1, index);
                        fclose(index);
                        struct stat file_stat;
                        stat("./static_site/index.html", &file_stat);
                        char last_modified[100];
                        strftime(last_modified, sizeof(last_modified), "%a, %d %b %Y %H:%M:%S %Z", localtime(&file_stat.st_mtime));

                        sprintf(response, "HTTP/1.1 200 OK\r\n"
                                          "Server: liso/1.1\r\n"
                                          "Date: %s\r\n"
                                          "Content-Length: %ld\r\n"
                                          "Content-Type: text/html\r\n"
                                          "Last-Modified: %s\r\n"
                                          "Connection: keep-alive\r\n\r\n%s",
                                          temp, strlen(htmlstr), last_modified, htmlstr);

                        FILE *log = fopen("Log.txt", "a");
                        fprintf(log, "[Date: %s][client: 127.0.0.1:38636]200 OK, responded for request GET\n", temp);
                        fclose(log);
                    }
                } else if (strcmp(request[k]->http_method, "HEAD") == 0) {
                    FILE *file = fopen("./static_site/index.html", "r");
                    if (file == NULL || strcmp(request[k]->http_uri, "/") != 0) {
                        sprintf(response, "HTTP/1.1 404 Not Found\r\n\r\n");
                    } else {
                        fclose(file);
                        struct stat file_stat;
                        stat("./static_site/index.html", &file_stat);
                        char last_modified[100];
                        strftime(last_modified, sizeof(last_modified), "%a, %d %b %Y %H:%M:%S %Z", localtime(&file_stat.st_mtime));

                        sprintf(response, "HTTP/1.1 200 OK\r\n"
                                          "Server: liso/1.1\r\n"
                                          "Date: %s\r\n"
                                          "Content-Length: %ld\r\n"
                                          "Content-Type: text/html\r\n"
                                          "Last-Modified: %s\r\n"
                                          "Connection: keep-alive\r\n\r\n",
                                          temp, (long)file_stat.st_size, last_modified);
                    }
                } else if (strcmp(request[k]->http_method, "POST") == 0) {
                    sprintf(response, "HTTP/1.1 200 OK\r\n\r\n");
                    FILE *log = fopen("Log.txt", "a");
                    fprintf(log, "[Date: %s][client: 127.0.0.1:38636]200 OK, responded for request POST\n", temp);
                    fclose(log);
                } else {
                    sprintf(response, "HTTP/1.1 501 Not Implemented\r\n\r\n");
                    FILE *log = fopen("Log.txt", "a");
                    fprintf(log, "[Date: %s][client: 127.0.0.1:38636]HTTP/1.1 501 Not Implemented\n", temp);
                    fclose(log);
                }
            } else {
                sprintf(response, "HTTP/1.1 505 HTTP Version Not Supported\r\n\r\n");
                FILE *log = fopen("Log.txt", "a");
                fprintf(log, "[Date: %s][client: 127.0.0.1:38636]HTTP/1.1 505 HTTP Version Not Supported\n", temp);
                fclose(log);
            }
        }
        
        strcat(buffer, response);
    }
    return buffer;
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

