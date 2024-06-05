/* C declarations used in actions */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "parse.h"

int main(int argc, char **argv){
  //Read from the file the sample
  int fd_in = open(argv[1], O_RDONLY);
  int index;
  char buf[8192];
	if(fd_in < 0) {
		printf("Failed to open the file\n");
		return 0;
	}
  int readRet = read(fd_in,buf,8192);
  int* request_num;
  *request_num = 0;
  //Parse the buffer to the parse function. You will need to pass the socket fd and the buffer would need to
  //be read from that fd
  Request** request = parse(buf,readRet,fd_in, request_num);
  //Just printing everything
  int k;
  for(k = 0;k<*request_num;k++){
    printf("Http Method %s\n",request[k]->http_method);
    printf("Http Version %s\n",request[k]->http_version);
    printf("Http Uri %s\n",request[k]->http_uri);
    for(index = 0;index < request[k]->header_count;index++){
      printf("Request Header\n");
      printf("Header name %s Header Value %s\n",request[k]->headers[index].header_name,request[k]->headers[index].header_value);
    }
    free(request[k]->headers);
    request[k]->headers = NULL;
    free(request[k]);
  }
  return 0;
}
