#include "mywidget.h"
#include "ui_mywidget.h"
#include <QImage>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    //自定义线程类对象，不能指定父对象
    myT = new Mythread;

    //创建子线程
    thread = new QThread(this);

    //把自定义线程添加到子线程
    myT->moveToThread(thread);

    //启动子线程，但是并没有启动线程处理函数
    thread->start();
    //线程处理函数必须通过signal -slost调用
    connect(ui->pushButton,&QPushButton::clicked,myT,&Mythread::drawImage);
    connect(myT,&Mythread::updateImage,this,&MyWidget::getImage);

    connect(this,&MyWidget::destroyed,this,&MyWidget::dealclose);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
//    p = new QPainter(this);
    painter.drawImage(10,10,image);
}
void MyWidget::getImage(QImage image){
    this->image = image;
    //更新窗口，间接调用paintEvent
    update();
}
void MyWidget::dealclose(){
    //退出子线程
    //回收
    thread->quit();
    thread->wait();

    delete myT;
}
