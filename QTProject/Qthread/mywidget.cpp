#include "mywidget.h"
#include "ui_mywidget.h"
#include <QDebug>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    thread = new MyThread(this);

    //定时器启动，出发函数
    connect(timer,&QTimer::timeout,this,&MyWidget::dealTimeout);
    connect(thread,&MyThread::isDone,this,&MyWidget::dealDone);
    //当右上角关闭出发的时候
    connect(this,&MyWidget::destroyed,this,&MyWidget::stopThread);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::stopThread(){
    thread->quit(); //停止线程
    thread->wait(); //等待线程处理完任务
}

void MyWidget::dealTimeout(){
    static int i = 0;
    i++;
    ui->lcdNumber->display(i);
}
void MyWidget::on_pushButton_clicked()
{
    if(timer->isActive() == false){
        timer->start(100);
    }

    //启动线程 处理数据
    thread->start();
}
void MyWidget::dealDone(){
    qDebug()<<"over";
    timer->stop();
}
