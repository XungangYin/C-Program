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


int doSomething(int conn){
     char recvbuff[1024];
      while(1){
	      memset(recvbuff,0,sizeof(recvbuff));
	      int ret = read(conn,recvbuff,sizeof(recvbuff));
	      if(ret ==0) //客户端关闭
	      {
		printf("client closed\n");
		break;
	      } else if(ret == -1){
		 ERR_EXIT("read");  //失败退出
	      }
	      fputs(recvbuff,stdout);
	      write(conn,recvbuff,ret);

	  }
}
   
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
    
    pid_t pid;
    while(1){
      if((conn= accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen))<0)
	  ERR_EXIT("accept");
      
      //打印客户端的地址信息
      printf("ip = %s,port = %d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
      pid  = fork();
      if(pid == -1)
	ERR_EXIT("fork");
      if(pid == 0){   //子进程,不监听
	  close(listenfd);
	  doSomething(conn);
	  exit(EXIT_SUCCESS); //客户端关闭时退出
      }else{          //父进程，不处理信息，管理链接套接口
	close(conn);
      }     
    }
  return 0;
}
