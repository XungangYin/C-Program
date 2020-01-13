#include "mythread.h"

MyThread::MyThread(QObject *parent) : QObject(parent)
{

    isStop = false;
}

void MyThread::myTimeout(){

    while (!isStop) {
        QThread::sleep(1);
        emit mySignal();
        qDebug()<<"子线程："<<QThread::currentThread();

        if(isStop){
            qDebug()<<"子线程结束";
        }
    }
}

void MyThread::setFlag(bool flag){
    isStop = flag;
}
