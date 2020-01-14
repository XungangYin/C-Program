#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include "mythread.h"
#include <QPainter>
#include <QThread>

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();

    //重写绘图
    virtual void paintEvent(QPaintEvent *event);

    void getImage(QImage image);

    void dealclose();
private:
    Ui::MyWidget *ui;
    QImage image;
    QThread *thread;
    Mythread *myT;
    QPainter *p;

};

#endif // MYWIDGET_H
