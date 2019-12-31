/**
 * 重要的函数.成功返回0，失败-1
 int socket(int domain,int type ,int protocol);
 int bind(int sockfd,const struct sockaddr *addr, socklen_t addrlen);
 int listen(int sockfd,int backlong)
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define ERR_EXIT(m) \
  do \
  { \
    perror(m);\
    exit(EXIT_FAILURE);\
   }while(0)

int main(void){
  
  int sock;
    if(( sock = socket(PF_INET,SOCK_STREAM,0)) < 0){
    	ERR_EXIT("socket");
    }
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(5188);
   // serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    //发起连接。和服务器端的conn建立了链接
    if( connect(sock,(struct sockadddr*)&serveraddr,sizeof(serveraddr)) < 0)
      ERR_EXIT("connect");
 
    char recvbuf[1024]={0};
    char sendbuf[1024]={0};
    while(fgets(sendbuf,sizeof(sendbuf),stdin)!= NULL){
            write(sock,sendbuf,strlen(sendbuf));
            int ret = read(sock,recvbuf,sizeof(recvbuf));
            if(ret == 0){
                printf("Server cloded\n");
                break; 
            } else{
    	        fputs(recvbuf,stdout);
                memset(recvbuf,0,sizeof(recvbuf));
            }
        }

     close(sock);
  return 0;
}
