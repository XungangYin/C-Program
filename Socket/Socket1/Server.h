#ifndef CHATROOM_SERVER_H
#define CHATROOM_SERVER_H

#include <string>
#include "Common.h"

using namespace std;

//服务器端类，用于处理客户请求
class Server {
public:
  Server();
  //初始化服务器设置
  void Init();
  
  //关闭服务器
  void Close();
  
  //启动服务器
  void Start();
  
private:
  //广播消息给所有客户端
  int SendBroadcastMessage(int clientfd);
  
  //服务器端的serverAddr信息
  //sockaddr_in是internet环境下套接字的地址形式。其和sockaddr主要用来处理网络通信的地址。其结构如下
  /*struct sockaddr_in {
        short int sin_family;           //地址族
	unsigned short int sin_port;    //16位TCP/UDP的协议端口号
	struct sin_addr;                 //32位的IP（IPv4）地址
	unsigned char sin_zero[8];      //为了让sockaddr和sockaddr_in保持大小相同而保留的空字节，一般用不到。
      }
    
   */ 
  struct sockaddr_in serverAddr;
  
  //创建监听的socket
  int listener;
  
   //epoll_create创建后的返回值
  int epfd;
  
  //客户端列表
  list<int> clients_list;
  
};

#endif