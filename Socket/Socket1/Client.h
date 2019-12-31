#ifndef CHATROOM_CLIENT_H
#define CHATROOM_CLIENT_H

#include <iostream>
#include <string>
#include "Common.h"

using namespace std;

//客户端类用来链接服务器发送和接受信息
class Client{
public:
  Client();
  
  //链接服务器
  void Connect();
  
  //断开链接
  void Close();
  
  //启动客户端
  void Start();
  
private:
  
  //当前链接服务器端创建的socket
  int sock;
  
  //当前进程
  int pid;
  
  //epoll_creat创建后的返回值
  int epfd;
  
  //创建管道，其中fd【0】用于父进程，fd【1】用于子进程
  int pipe_fd[2];
  
  //表示客户端是否正常工作
  bool isClientwork;
  
  //聊天信息
  Msg msg;
  
  //结构体转换为字符串
  char send_buf[BUF_SIZE];
  char recv_buf[BUF_SIZE];
  
  //用户链接的服务器IP和port
  struct sockaddr_in serverAddr;
};



#endif