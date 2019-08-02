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
  
  int listenfd;
    if(( listenfd = socket(PF_INET,SOCK_STREAM,0)) < 0){
    	ERR_EXIT("socket");
    }
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(5188);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0)
      ERR_EXIT("bind");
      
    //一旦监听，则为被动套接字(只能接受连接，调用accep函数之前调用)
    if(listen(listenfd,100)< 0)
      ERR_EXIT("listen");
    
    //对方的地址
    struct sockaddr_in peeraddr;
    socklen_t peerlen = sizeof(peeraddr);
    int conn;  //返回的一个主动套接字
    if((conn= accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen))<0)
        ERR_EXIT("accept");

    char recvbuff[1024];
    while(1){
            memset(recvbuff,0,sizeof(recvbuff));
            int ret = read(conn,recvbuff,sizeof(recvbuff));
            fputs(recvbuff,stdout);
            write(conn,recvbuff,ret);

        }
    close(listenfd);    
  return 0;
}
