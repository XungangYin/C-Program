#include "mythread.h"

MyThread::MyThread(QObject *parent) : QThread(parent)
{

}

void MyThread::run(){
    //说5s
    sleep(5);

    emit isDone(); //完成之后发射信号
}
