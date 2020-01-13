#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QTimer>
#include "mythread.h"

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();

private slots:
    void on_pushButton_clicked();
    void dealTimeout();
    void dealDone();
    void stopThread();

private:
    Ui::MyWidget *ui;
    QTimer *timer;
    MyThread *thread;
};

#endif // MYWIDGET_H
