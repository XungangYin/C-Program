/*
  c++类的成员函数创建线程：
  注意点大概如下：
  1. thread的第一个参数一定是 &类名::函数名 的形式
  2. thread的第二个参数一定是类实例的指针
  3. thread的之后的参数是函数所需要的参数
 */


#include <iostream>
#include <thread>
#include <mutex>

//int tickets  = 10;
std::mutex m;

using namespace std;
class SellTickets
{
public:
  void init();
  void thread1();
  void thread2();

private:
  int tickets = 20;
};

void SellTickets::thread1(){
  while(true){
    if(tickets >0){
      m.lock();  //线程锁，防止多线程访问同一个变量
      cout<<tickets--<<endl;
      m.unlock();
    }
    else
      break;
  }
  
}
void SellTickets::thread2(){
  while(true){
    if(tickets >0)
      cout<<tickets--<<endl;
    else
      break;
  }
}

void SellTickets::init(){
  std::thread tA(&SellTickets::thread1,this);//?为什么没有this会报错
  std::thread tB(&SellTickets::thread2,this);

  tA.detach();
  tB.detach();
}
int main(int argc,char **argv){

  SellTickets *s = new SellTickets;

  s->init();

 pthread_exit(NULL);
  cout<<"this is main hanshu1"<<endl;
  delete s;
  return 0;
}
