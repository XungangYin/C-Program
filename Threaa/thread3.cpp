/*
  pthread_join函数：将子线程合并入主线程，主线程阻塞，直到该子线程结束并回收子线程资源
 */
#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;
#define NUM 5

void *wait(void *t){
  //int i;
  long tid;
  tid = (long)t;
  sleep(0.1);
  cout<<"Sleeping in thread "<<endl;
  cout<<"Thread with id: "<<tid<<"...exiting"<<endl;
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
    rc = pthread_join(threads[i],&status);  //阻塞主线程
    if(rc){
      cout<<"Error:unable to join"<<rc<<endl;
    }

    cout<<"Main:completed thread id:"<<i;
    cout<<"  exiting with status:"<<status<<endl;
    
  }

  cout<<"Main:program exiting "<<endl;
  pthread_exit(NULL);
}
