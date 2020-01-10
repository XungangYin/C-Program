#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QPixmap>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //去边框
    this->setWindowFlags(Qt::FramelessWindowHint |  windowFlags());
    //把背景设为透明
    setAttribute(Qt::WA_TranslucentBackground);
}

void Widget::paintEvent(QPaintEvent *event){
    QPainter p(this); //定义画家
//    p.begin(this);
//    p.drawPixmap(0,0,this->width(),this->height(),QPixmap("./test.png"));
    //绘制背景图
    p.drawPixmap(rect(),QPixmap(":/new/test.png"));

//    p.end();
}



Widget::~Widget()
{
    delete ui;
}
