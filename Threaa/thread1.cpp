#include <pthread.h>
#include <iostream>
#include <unistd.h>

using namespace std;

#define NUM 10
//
//线程函数，返回值无类型，参数无类型
void *say_hello(void * args){
  cout<<"Hello World"<<endl;
  sleep(0.1);
  return 0;
}

int main(){
  //定义线程的id变量，多个变量使用数组
  pthread_t tids[NUM];
  for(int i = 0; i< NUM;i++){
    
    //参数：创建的线程id,线程参数，调用函数，传入函数的参数
    int ret = pthread_create(&tids[i],NULL,say_hello,NULL); //创建并执行线程,成功返回0，否则返回错误代码
    if(ret !=0 ){
      cout<<"pthread_create error"<<endl;
    }
  }
     for(int i = 0;i < 10;i++){
          cout<<"this main thread"<<endl;
      }
    
    //等各个线程退出后，进程才结束，否则进程强制结束了，线程可能还没反应过来；
    //退出当前主线程,并返回NULL
    cout<<"asdfafd"<<endl;
    pthread_exit(NULL);
    cout<<"145r"<<endl;
}
