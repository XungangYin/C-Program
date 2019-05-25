/*
    一些公共函数接口
*/
#ifndef CHATROOM_COMMON_H
#define CHATROOM_COMMON_H

#include <iostream>
#include <list>
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

//默认服务器端IP地址
#define SERVER_IP "127.0.0.1"

//服务器端口号
#define SERVER_PORT 8888

//epoll支持的最大句柄
#define EPOLL_SIZE 5000

//缓冲区大小
#define BUF_SIZE 0XFFFF

//新用户登录后的欢迎信息
#define SERVER_WELCOME "Welcome to join the root!"

//其他用户收到信息的前缀
#define SERVER_MESSAGE "ClientID %d say >> %s"
#define SERVER_PRIVATE_MESSAGE "Client %d say to you privately >> s%"
#define SERVER_PRIVATE_ERROR_MESSAGE "Client %d is not in the chat room!"

//退出系统
#define EXIT "EXIT"

//提醒你是聊天室中的唯一客户
#define CAUTION "This is only one in the char room!"

/* epoll_event的结构体定义
struct epoll_event{
  __uint32_t events;
  epoll_data_t data;  //是一个联合体
};
*/

//注册新的fd到epoll中
//参数enable_et表示是否启用ET模式
static void addfd(int epollfd,int fd,bool enable_et){
  struct epoll_event ev;
  ev.data.fd  = fd;
  ev.events = EPOLLIN; //表示对应的链接上有数据可以读出
  if(enable_et){
    ev.events = EPOLLIN | EPOLLET;  //将触发方式设置为边缘触发，默认LT
  }
  //添加新的事件到epollfd中，返回0成功，否则返回-1,fd表示待监测的链接套接字；
  epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev); 
    // 设置socket为非阻塞模式
    // 套接字立刻返回，不管I/O是否完成，该函数所在的线程会继续运行
    //eg. 在recv(fd...)时，该函数立刻返回，在返回时，内核数据还没准备好会返回WSAEWOULDBLOCK错误代码

  //fcntl函数针对fd描述符进行控制，第二个参数表示操作
  fcntl(fd,F_SETFL,fcntl(fd,F_GETFD,0) | O_NONBLOCK);
  
  printf("fd added to epoll! \n\n");  
  
}
//定义信息结构，在服务端和客户端之间传送
struct Msg
{
    int type;
    int fromID;
    int toID;
    char content[BUF_SIZE];
 
};

#endif
