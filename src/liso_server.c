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


#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

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
    int server_sockfd, client_sockfd;
    fd_set readfds, testfds;
    struct sockaddr_in server_address, client_address;
    char buf[BUF_SIZE*2];
    char *pro_buf;
    socklen_t client_len;
    ssize_t readret;
    int ready_num, fd, nread;

    fprintf(stdout, "----- Liso Server -----\n");

    if ((server_sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Failed creating socket.\n");
        return EXIT_FAILURE;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(ECHO_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // 绑定
    if (bind(server_sockfd, (struct sockaddr *) &server_address, sizeof(server_address))) {
        close_socket(server_sockfd);
        fprintf(stderr, "Failed binding socket.\n");
        return EXIT_FAILURE;
    }

    // 监听
    if (listen(server_sockfd, 5)) {
        close_socket(server_sockfd);
        fprintf(stderr, "Error listening on socket.\n");
        return EXIT_FAILURE;
    }

    FD_ZERO(&readfds);
    FD_SET(server_sockfd, &readfds);

    while (1) {
        testfds = readfds;
        printf("server waiting\n");

        ready_num = select(FD_SETSIZE, &testfds, NULL, NULL, NULL);
        if (ready_num < 1) {
            perror("server");
            exit(1);
        }

        for (fd = 0; fd < FD_SETSIZE; fd++) {
            if (FD_ISSET(fd, &testfds)) {
                if (fd == server_sockfd) {
                    client_len = sizeof(client_address);
                    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
                    if (client_sockfd == -1) {
                        perror("accept");
                        continue;
                    }
                    FD_SET(client_sockfd, &readfds);
                    printf("adding client on fd %d\n", client_sockfd);
                } else {
                    ioctl(fd, FIONREAD, &nread);
                    if (nread == 0) {
                        close(fd);
                        FD_CLR(fd, &readfds);
                        printf("removing client on fd %d\n", fd);
                    } else {
                        readret = read(fd, buf, BUF_SIZE);
                        if (readret > 0) {
                            pro_buf = process(buf, readret);
                            write(fd, pro_buf, strlen(pro_buf));
                            free(pro_buf);
                            memset(buf, 0, BUF_SIZE);
                        } else if (readret == -1) {
                            perror("read");
                            close(fd);
                            FD_CLR(fd, &readfds);
                        }
                        printf("serving client on fd %d\n", fd);
                    }
                }
            }
        }
    }
    return EXIT_SUCCESS;
}