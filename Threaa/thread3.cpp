/*
  pthread_join函数：将子线程合并入主线程，主线程阻塞，直到该子线程结束并回收子线程资源
 */
#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;
#define NUM 2

void *wait(void *t){
  //int i;
  long tid;
  tid = (long)t;
  cout<<"Sleeping in thread "<<endl;
  cout<<"Thread with id: "<<tid<<"...exiting"<<endl;
 // sleep(3);
  while(1){
    cout<<"这是一个测试:"<<endl;    
  }
  pthread_exit(NULL);
}

int main(){
  int rc;
  int i;
  pthread_t threads[NUM];
  pthread_attr_t attr;
  void *status;

  //初始化并设置线程为可连接的
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

  for(i = 0;i < NUM;i++){
    cout<<"main():creating thread, "<<i<<endl;
    rc = pthread_create(&threads[i],NULL,wait,(void*)&i);
    if(rc){
      cout<<"Error:unable to create threads"<<endl;
    }
  }

  //程
  for(i=0;i<NUM;i++){
    //rc = pthread_join(threads[i],&status);  //阻塞主线程,等待指定线程的结束
    if(rc){
      cout<<"Error:unable to join"<<rc<<endl;
    }

    cout<<"Main:completed thread id:"<<i;
    cout<<"  exiting with status:"<<status<<endl;
    
  }

  cout<<"Main:program exiting "<<endl;
  //pthread_exit(NULL); //主线程退出,由它创建的线程并不退出
 // exit(0);//主进程退出，在该进程创建的所有线程退出
  sleep(2);
  pthread_cancel(threads[0]);//取消执行中的线程
  pthread_cancel(threads[1]);
}
