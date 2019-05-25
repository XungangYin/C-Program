#include <iostream>
#include <memory>
using namespace std;
class A{
public:
  A(int a):value(a){
    cout<<value<<" 调用构造函数"<<endl;
  }

  ~A(){
    cout<<value<<"调用析构函数"<<endl;
  }

  void display(){
    cout<<"do somethings"<<endl;
  }
  
  int value;
};


int main(int argc,char **argv)
{
  {
    //多个shared_ptr指向同一处资源，当所有shared_ptr都全部释放时，该处资源才释放
    cout<<"shared_ptr例子"<<endl;
    shared_ptr<A> a;  //只是创建了一个空指针
    a = make_shared<A>(0); //真正分配一块内存
    cout<<"..."<<endl;

    shared_ptr<A> b(new A(1));
    cout<<"..."<<endl;
    
    auto c = std::make_shared<A>(2);
    cout<<"..."<<endl;
    if(!a)
      {
	cout<<"2134123"<<endl;
	a = b;
	a.reset();
      }
 
    A *_a = new A(10);
    A _b(11);
    cout<<"asdfasf"<<endl;
    delete _a;  //如果注释掉，则_a不析构，造成内存泄露.因为A不是智能指针
    _a = nullptr;//防止成为野指针

    cout<<"结束shared_ptr"<<endl;
  }
  
  {
    //unique_ptr独占所有权
    cout<<"开始unique_ptr"<<endl;
    std::unique_ptr<A> test1(new A(1));
    //    std::unique_ptr<A> test2 = test1; //编译期出错，不允许指针的复制。
    std::unique_ptr<A> test3 = std::move(test1); //转移test1的所有权。并不调用构造函数
    //    tes1->display();  //编译期报错，test1没有被声明
    test3->display();

    
    cout<<"结束unique_ptr"<<endl;
  }


  {
    //weak_ptr只有访问权，没有所有权(没有对象的生命控制权)
    cout<<"开始weak_ptr"<<endl;
    std::shared_ptr<A> test1(new A(1));
    std::weak_ptr<A> test2 = test1;

    //    test2->display();//编译出错,weak_ptr没有重载->
    test1->display();
    
    cout<<"ending weak_ptr"<<endl;
    
  }

  
  return 0;
}
