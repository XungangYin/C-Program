#include <iostream>
#include "Server.h"

using namespace std;

//服务器端的构造函数
Server::Server()
{
  //初始化服务器地址和端口
  serverAddr.sin_family = PF_INET; //协议族
  //htons():将主机字节序转化为网络字节序的整数值
  serverAddr.sin_port = htons(SERVER_PORT);
  serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
  //初始化socket
  listener  = 0;
  //epool fd
  epfd = 0;
 
}

//初始化服务端并启动监听
void Server::Init()
{
  cout<<"Init Server..."<<endl;
  
  //创建监听的socket
  //参数1：协议族，参数2：指定socket类型，这里用了流式socket，针对与TCP连接应用。参数3：指定协议，0表示使用参数2对应的默认协议
  //成功返回新创建的套接字的描述符，失败返回-1；
  listener = socket(PF_INET,SOCK_STREAM,0);
  if(listener <0 ){
    perror("listener");
    exit(-1); 
  }
  
  //绑定地址。将本地地址结构和socket接口捆绑，捆绑缓冲区的长度为参数3所指定，捆绑成功返回0
  if(bind(listener,(struct sockaddr *)&serverAddr,sizeof(serverAddr))<0){
    perror("bind error");
    exit(-1);
  }
  
  //监听服务器。
  /*listen()函数：使主动链接套接口变为被动连接套接口，从而使得一个进程可以接受其他进程的请求，进而成为一个服务器进程
   * 在TCP服务器编程中，该函数把进程变为一个服务器，并指定相应的套接字变为被动连接。
   * 该函数的调用一般在bind（）之后和accept（）之前
   * 参数1：套接字，参数2：等待连接的最大长度。成功返回0
   */
  int ret = listen(listener,5);
  if(ret < 0){
    perror("listen error");
    exit(-1);
  }
  
  cout<<"Start to listen:"<<SERVER_IP << endl;
  
  //在内核中创建事件列表，epfd是一个句柄.
  //生成一个epoll实例，参数表示：在该实例(epoll fd)上能关注的最大socket fd的数量。fd = file descriptor文件描述符
  epfd = epoll_create(EPOLL_SIZE);
  if(epfd <0){
    perror("epfd error");
    exit(-1);
  }
  
  //向事件表里添加监听事件。将socket fd注册到epoll fd中，并将触发方式设置为ET提高效率
  addfd(epfd,listener,true);
  
}

//关闭服务器并清理事件描述符
void Server::Close()
{
  //关闭socket
  close(listener);
  
  //关闭epoll监听
  close(epfd);
}

//发送广播消息给所有客户端
int Server::SendBroadcastMessage(int clientfd)
{
  //buf[]接受新消息
  //message【】保存格式化消息
  char recv_buf[BUF_SIZE];
  char send_buf[BUF_SIZE];
  
  Msg msg;
  //置字符串前BUF_SIZE个字节为0
  bzero(recv_buf,BUF_SIZE);
  //接受消息
  cout<<"read from client (cliendID =  "<<clientfd<<")"<<endl;
  
  /*无论是服务器还是客户端都用recv函数从对方接受数据.
   * 参数1：指定接受端套接字描述符;参数2：指明缓冲区；参数3：缓冲区的大小；参数4：一般为 0；
   */
  int len = recv(clientfd,recv_buf,BUF_SIZE,0);
  //清空结构体，把接收到的字符串转换为结构体
  memset(&msg,0,sizeof(msg));
//  cout<<"msg is:"<<msg.content[0]<<endl;
  memcpy(&msg,recv_buf,sizeof(msg));
 // cout<<"msg1 is:"<<msg.content[0]<<endl;
  
  //判断收到的消息是私聊还是群聊
  msg.fromID = clientfd;
  if(msg.content[0] == '\\' && isdigit(msg.content[1])){  //isdigit函数用于检查参数是否是十进制数字字符
    msg.type = 1;
    //将对应的字符数字转化为数字。例如：‘6’-‘0’ = 6；
    msg.toID = msg.content[1]-'0';
    //从结尾后两个开始复制. 为啥这么做？？
    memcpy(msg.content,msg.content+2,sizeof(msg.content));
  } 
  else
    msg.type = 0;
    
  //如果客户端关闭了链接
  if(0 == len){
    close(clientfd);
    //在客户端列表中删除该客户端
    clients_list.remove(clientfd);
    cout<<"ClientID ="<<clientfd
      <<" closed .\nnow there are "<<clients_list.size()<<" client in the char room"<<endl;
  }
  //发送广播消息给所有客户端
  else { 
    //判断聊天室是否还有其他客户端
    if(1 == clients_list.size()){
      //发送提示消息
      memcpy(&msg.content,CAUTION,sizeof(msg.content));
      bzero(send_buf,BUF_SIZE);
      memcpy(send_buf,&msg,sizeof(msg));
      
      /*无论是客户端还是服务器程序都用send函数发送数据，客户端用它发送请求，服务器用它发送应答
       * 参数1：指定发送端套接字fd；参数2：存放数据的缓冲区；参数3：发送数据的字节大小；参数4：一般为0
       */
      send(clientfd,send_buf,sizeof(send_buf),0);
      return len;
    }
    
    //存放格式化后的信息
    char format_message[BUF_SIZE];
    //群聊
    if(0 == msg.type){
      //格式化发送的消息内容 #define SERVER_MESSAGE
      sprintf(format_message,SERVER_MESSAGE,clientfd,msg.content);
      memcpy(msg.content,format_message,BUF_SIZE);
      //遍历客户端列表，依次发送消息,需要判断不要给来源客户端发
      list<int>::iterator it;
      for(it = clients_list.begin();it != clients_list.end();++it){
	if(*it != clientfd){
	  //把发送的结构体转化为字符串
	  bzero(send_buf,BUF_SIZE);
	  memcpy(send_buf,&msg,sizeof(msg));
	  if(send(*it,send_buf,sizeof(send_buf),0)<0)
	    return -1;
	}
      }
      
    }
    
   //私聊
    if(1 == msg.type){
      bool private_offline = true;
      sprintf(format_message,SERVER_PRIVATE_MESSAGE,clientfd,msg.content);
      memcpy(msg.content,format_message,BUF_SIZE);
      //遍历客户端列表依次发送消息，需要判断不要给来源客户端发
      list<int>::iterator it;
      for(it = clients_list.begin();it != clients_list.end();++it){
	if(*it == msg.toID){
	  private_offline = false;
	  //把发送的结构体转换为字符串
          bzero(send_buf, BUF_SIZE);
          memcpy(send_buf,&msg,sizeof(msg));
          if( send(*it,send_buf, sizeof(send_buf), 0) < 0 ) {
              return -1;
           }
 
	}
      }
     
     //如果私聊对象不在线
      if(private_offline){
	sprintf(format_message,SERVER_PRIVATE_ERROR_MESSAGE,msg.toID);
        memcpy(msg.content,format_message,BUF_SIZE);
        bzero(send_buf,BUF_SIZE);
        memcpy(send_buf,&msg,sizeof(msg));
        if(send(msg.fromID,send_buf,sizeof(send_buf),0)<0)
               return -1;
      }
     
    }
   
  }
    
  return len;

}

//启动服务器端
void Server::Start()
{
  //epoll事件队列
  static struct epoll_event events[EPOLL_SIZE];
  
  //初始化服务端。
  Init();
  
  while(1){
    //epoll_events_count表示就绪事件的数目
    /*epoll_wait():wait  for  an  I/O  event on an epoll file descriptor
     * 参数1：epfd句柄；参数2：事件的集合；参数3：事件集合的大小；参数4：0 表示立即返回，不阻塞，-1 表示阻塞等到相应事件发生时返回。
     * 函数成功返回准备就绪的文件个数，失败返回-1。
     */
    int epoll_events_count = epoll_wait(epfd,events,EPOLL_SIZE,-1);
    
    if(epoll_events_count < 0){
      perror("epoll failuer!");
      break;
    }
    cout<<"epoll_events_count = "<<epoll_events_count<<endl;
    //处理就绪事件
    for(int i = 0; i < epoll_events_count;++i){
      int sockfd = events[i].data.fd;
      //新用户链接. 为什么他俩相等就是新用户的连接？
      if(sockfd == listener){
	struct sockaddr_in client_address;
	socklen_t client_addrLength = sizeof(struct sockaddr_in);
	/*accept():接受客户端的连接请求，成功返回与参数1同类型的套接字fd；
	 * 参数1：套接字fd，该函数从参数1的等待连接队列中抽取第一个连接，如果队列中无等待连接，且套接口为阻塞方式，则函数阻塞调用进程直至新的连接出现；
	 * 参数2：结构体指针，存放客户端的地址信息；参数3：为参数2指向区域的长度，在函数调用后被设置为实际地址信息长度
	 * 返回值：新的套接字fd，代表的是和客户端的新的连接
	 */
	int clientfd = accept(listener,(struct sockaddr *)&client_address,&client_addrLength);
	cout << "client connection from: "
                     << inet_ntoa(client_address.sin_addr) << ":"
                     << ntohs(client_address.sin_port) << ", clientfd = "
                     << clientfd << endl;
	addfd(epfd,clientfd,true);
	
	//服务器端用list保存用户链接
	clients_list.push_back(clientfd);
	cout << "Add new clientfd = " << clientfd << " to epoll" << endl;
        cout << "Now there are " << clients_list.size() << " clients in the chat room" << endl;
	
	//服务器端发送欢迎消息
	cout<<"Welcome message to Client"<<"\n"<<endl;
	char message[BUF_SIZE];
	bzero(message,BUF_SIZE);
	sprintf(message,SERVER_WELCOME,-1);
	//printf("message is:%s",SERVER_WELCOME);
	
	int ret = send(clientfd,message,BUF_SIZE,0);
	if(ret <0 ){
	  perror("send error");
	  Close();
	  exit(-1);
	}
      }
      //处理用户发来的消息
      else{
	int ret = SendBroadcastMessage(sockfd);
	if(ret < 0){
	  perror("error");
	  Close();
	  exit(-1);
	}
      }
    }
  }

  //关闭服务
  Close();
}




