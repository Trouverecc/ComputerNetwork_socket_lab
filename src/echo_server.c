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
#include "parse.h"

#define ECHO_PORT 9999
#define BUF_SIZE 4096
//端口号，不再更改

Request * parse(char *buffer, int size, int socketFd);

char* process(char *buff, int size) {
    Request* request;
    request = parse(buff, size, 0);
    if(request!=NULL) {
        if(!strcmp(request->http_method, "GET") || !strcmp(request->http_method,"HEAD") || !strcmp(request->http_method,"POST")){
            return buff;
        }
        else{
            yylex_destroy(buff);
            buff = "HTTP/1.1 501 Not Implemented\r\n\r\n";
        }
    }
    else{
        yylex_destroy(buff);
        buff = "HTTP/1.1 400 Bad request\r\n\r\n";
    }
}


int close_socket(int sock)
{
    if (close(sock))
    {
        fprintf(stderr, "Failed closing socket.\n");
        return 1;
    }
    return 0;
}


int main(int argc, char* argv[])
{
    int sock, client_sock;
    ssize_t readret;
    socklen_t cli_size;
    struct sockaddr_in addr, cli_addr;
    char buf[BUF_SIZE];
    char *pbuf;

    fprintf(stdout, "----- Echo Server -----\n");
    
    /* all networked programs must create a socket */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Failed creating socket.\n");
        return EXIT_FAILURE;
    }

    addr.sin_family = AF_INET;        //IPv4
    addr.sin_port = htons(ECHO_PORT); //port 9999
    addr.sin_addr.s_addr = INADDR_ANY;//ip=0.0.0.0(本机)

    /* servers bind sockets to ports---notify the OS they accept connections */
    //绑定
    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)))
    {
        close_socket(sock);
        fprintf(stderr, "Failed binding socket.\n");
        return EXIT_FAILURE;
    }

    //监听
    if (listen(sock, 5))
    {
        close_socket(sock);
        fprintf(stderr, "Error listening on socket.\n");
        return EXIT_FAILURE;
    }

    /* finally, loop waiting for input and then write it back */
    while (1)
    {
        cli_size = sizeof(cli_addr);
        if ((client_sock = accept(sock, (struct sockaddr *) &cli_addr,
                                    &cli_size)) == -1)
        {
            close(sock);
            fprintf(stderr, "Error accepting connection.\n");
            return EXIT_FAILURE;
        }

        readret = 0;

        while((readret = recv(client_sock, buf, BUF_SIZE, 0)) >= 1)
        {
            pbuf = process(buf, strlen(buf));
            printf("server: %s\n", pbuf);
            send(client_sock, pbuf, strlen(pbuf), 0);
            memset(buf, 0, BUF_SIZE);
        } 

        if (readret == -1)
        {
            close_socket(client_sock);
            close_socket(sock);
            fprintf(stderr, "Error reading from client socket.\n");
            return EXIT_FAILURE;
        }

        if (close_socket(client_sock))
        {
            close_socket(sock);
            fprintf(stderr, "Error closing client socket.\n");
            return EXIT_FAILURE;
        }
    }

    close_socket(sock);

    return EXIT_SUCCESS;
}
