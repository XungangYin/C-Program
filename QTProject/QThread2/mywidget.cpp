#include "mywidget.h"
#include "ui_mywidget.h"

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    myT = new MyThread();//不能指定父对象,若指定，则不能movetothread（）

    //创建子线程
    thread = new QThread(this);

    //把自定义的线程加入到子线程
    myT->moveToThread(thread);

    connect(myT,&MyThread::mySignal,this,&MyWidget::dealSignal);


    qDebug()<<"主线程："<<QThread::currentThread();

    connect(this,&MyWidget::startThread,myT,&MyThread::myTimeout);

    connect(this,&MyWidget::destroyed,this,
            [=](){
                    on_pushButton_2_clicked();
                    delete myT;
                }
            );
    //线程处理函数内不允许操作界面

    //connect()函数第五个参数的作用
    //多线程才有意义
    //默认：
    /*
        如果是单线程，默认使用直接方式；
        若果是多线程，默认使用队列；
        队列：槽函数所在的线程和接受者同一个；
        直接：槽函数所在线程和发送者同一个
    */
}

MyWidget::~MyWidget()
{
    delete ui;
    delete myT;
}
void MyWidget::dealSignal(){
    static int i = 0;
    i++;
    ui->lcdNumber->display(i);

}

void MyWidget::on_pushButton_clicked()
{
    if(thread->isRunning() == true){
        return ;
    }
    //启动线程，但是没启动线程处理函数
    thread->start();
    myT->setFlag(false);
    //不能直接调用线程处理函数
    //直接调用，导致线程处理函数和主线程在同一个线程
    //    myT->myTimeout();
    //只能通过signale- slot方式调用
    emit startThread();
}

void MyWidget::on_pushButton_2_clicked()
{
    if(thread->isRunning() == false){
        return ;
    }
    myT->setFlag(true);
    thread->quit();
    thread->wait();
}
