/*
  定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，所有依赖于它的对象都得到通知并被自动更新。当一个对象发生了变化，关注它的对象就会得到通知；这种交互也称为发布-订阅(publish-subscribe)。目标是通知的发布者，它发出通知时并不需要知道谁是它的观察者。
 */

#include <iostream>
#include <list>

using namespace std;
//观察者：提供更新函数
class Observer
{
public:
    virtual void update(int) =0; //纯虚函数
};

//目标：提供注册/反注册 观察者的函数，通知发出的函数
class Subject
{
public:
    virtual void Attach(Observer *) = 0;
    virtual void Detach(Observer *) = 0;
    virtual void Notify() = 0;
};


//具体观察者1
class ConcreteObserver : public Observer
{
public:
    ConcreteObserver(Subject *sub):_sub(sub){} //观察者观察目标
    void update(int value){
        cout<<"更新Concrete 1:"<<value<<endl;
    }

private:
    Subject *_sub;

};

class ConcreteObserver2 : public Observer
{
public:
    ConcreteObserver2(Subject *sub):_sub(sub){}

    void update(int value){
        cout<<"更新Concrete2 :"<<value<<endl;
    }
private:
    Subject *_sub;
};

//具体目标
class ConcreteSubject : public Subject
{
public:
    ConcreteSubject(){}
    void Attach(Observer *pObserver);
    void Detach(Observer *pObserver);
    void Notify();
    void SetState(int s){state = s;}
private:
    int state;
    list<Observer *> l_Observer;
};
void ConcreteSubject::Attach(Observer *pObserver)
{
    l_Observer.push_back(pObserver);
}

void ConcreteSubject::Detach(Observer *pObserver)
{
    l_Observer.remove(pObserver);
}

void ConcreteSubject::Notify(){
    std::list<Observer *>::iterator it = l_Observer.begin();
    while(it != l_Observer.end()){
        (*it)->update(state);
        it++;
    }
}
int main(int argc,char **argv){
    ConcreteSubject *pSubject = new ConcreteSubject;

    ConcreteObserver *pObserver1 = new ConcreteObserver(pSubject);
    ConcreteObserver2 *pObserver2 = new ConcreteObserver2(pSubject);

    pSubject->SetState(10);

    pSubject->Attach(pObserver1);
    pSubject->Attach(pObserver2);

    pSubject->Notify();

    pSubject->Detach(pObserver1);
    pSubject->SetState(4);
    pSubject->Notify();



    delete pSubject;
    delete pObserver1;
    delete pObserver2;
  return 0;
}
