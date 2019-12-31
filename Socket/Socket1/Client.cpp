#include "Client.h"

using namespace std;

Client::Client()
{
  //初始化服务区链接的地址和端口
  serverAddr.sin_family = PF_INET;
  serverAddr.sin_port = htons(SERVER_PORT);
  serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
  
  //初始化socket
  sock = 0;
  
  //初始化进程号
  pid = 0;
  
  //客户端
  isClientwork = true;
  
  //epoll fd
  epfd = 0;
}

//链接服务器
void Client::Connect()
{
  cout<<"Connect Server: "<<SERVER_IP<<":"<<SERVER_PORT<<endl;
  //创建socket
  sock = socket(PF_INET,SOCK_STREAM,0);
  if(sock <0){
    perror("sock error");
    exit(-1);
  }
  
  //链接服务器端
  /*connect():函数用于创建与指定外部端口的连接。
   * 参数1：指定一个未连接的流类套接口（SOCK_STREAM）；参数2：指向要连接的套接字的sockaddr的指针；参数3：字节长度
   * 成功返回0，否则返回-1；
   * 注意：若参数1指定的socket未被绑定，则系统赋给本地关联一个唯一的值，且设置套接口为已绑定。
   */
  if(connect(sock,(struct sockaddr *)&serverAddr,sizeof(serverAddr))<0){
    perror("connect error");
    exit(-1);
  }
  
  //创建管道
  /*创建管道端口，参数：文件描述符的数组(只有两个元素)
   * pipe_fd[0]:管道读端
   * pipe_fd[1]:管道写端
   */
  if(pipe(pipe_fd) < 0){
    perror("pipe error");
    exit(-1);
  }
  
  //创建epoll
  epfd = epoll_create(EPOLL_SIZE);
  if(epfd <0){
    perror("epfd error");
    exit(-1);
  }
  
  //将socket和管道读取端描述符都添加到内核事件表中
  addfd(epfd,sock,true);
  addfd(epfd,pipe_fd[0],true);

}

//关闭链接
void Client::Close()
{
  if(pid){
    //关闭夫进程的管道和sock
    close(pipe_fd[0]);
    close(sock);
  }else{
    close(pipe_fd[1]);
  }
}

//启动客户端
void Client::Start()
{
  //epoll 事件队列
  static struct epoll_event events[2];
  //链接服务器
  Connect();
  
  //创建子进程，并分配资源
  /*fork()执行后产生两个进程，父进程函数返回子进程的ID，子进程的fork()返回0；
   */
  pid  = fork();
  //若创建失败则退出
  if(pid < 0){
    perror("fork error");
    close(sock);
    exit(-1);
  }else if(0 == pid){
    //进入子进程执行流程
    //子进程负责写入管道，因此关闭读端
    close(pipe_fd[0]);
    
    //输入exit可以退出聊天室
    cout<<"Please input 'exit' to exit the chat room"<<endl;
  //  cout<<"\\ + ClientID to private chat"<<endl;
    //如果客户端运行正常则不断读取输入发送给服务端
    while(isClientwork){
      //清空结构体
      memset(msg.content,0,sizeof(msg.content));
     //从标准输入（键盘）读入字符
      fgets(msg.content,BUF_SIZE,stdin);
      //客户端输出exit 退出
      //strncasecmp()函数用于比较参数1和参数2字符串的前n(参数3)个字节，比较时忽略大小写的差异
      if(strncasecmp(msg.content,EXIT,strlen(EXIT)) == 0){
	isClientwork = false;
      }
      //子进程将信息写入管道
      else{
	//清空发送的缓存
	memset(send_buf,0,BUF_SIZE);
	//结构体转化为字符串
	memcpy(send_buf,&msg,BUF_SIZE);
	//向管道写入数据pipe_fd[1]是写端。
	if(write(pipe_fd[1],send_buf,sizeof(send_buf))<0){
	  perror("fork error");
	  exit(-1);
	}
      }
    }
  }else{
    //pid > 0 父进程
    //父进程负责读取管道数据，因此先关闭写端
    close(pipe_fd[1]);
    //主循环 epoll_wait
    while(isClientwork){
      //等待该epfd的读写事件
      int epoll_events_count = epoll_wait(epfd,events,2,-1);
      
      //处理就绪事件
      for(int i = 0; i< epoll_events_count;++i){
	memset(recv_buf,0,sizeof(recv_buf));
	//服务端发来消息
	if(events[i].data.fd == sock){
	  //接受服务器端广播消息
	  int ret = recv(sock,recv_buf,BUF_SIZE,0);
	  //清空结构体
	  memset(&msg,0,sizeof(msg));
	  //将发来的消息转化为结构体
	  memcpy(&msg,recv_buf,sizeof(msg));
	  
	  //ret = 0服务器关闭
	  if(ret == 0){
	    cout<<"Server closed connection:"<<sock<<endl;
	    close(sock);
	    isClientwork = 0;
	  }else{
	    cout<<msg.content<<endl;
	  }
	}
	//子进程写入事件发生，父进程处理并发送服务端
	else{
	  //父进程从管道中读取数据
	  int ret = read(events[i].data.fd,recv_buf,BUF_SIZE);
	  if(0 == ret){
	    isClientwork = 0;
	  }else{
	    //将从管道中读取的字符串发送给服务端
	    send(sock,recv_buf,sizeof(recv_buf),0);
	  }
	}
      }
    }
  }
  
  //退出线程
  Close();

}



