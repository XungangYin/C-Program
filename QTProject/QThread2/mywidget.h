#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QDebug>
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

    void dealSignal();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void startThread();
private:
    Ui::MyWidget *ui;
    MyThread *myT;
    QThread *thread;
};

#endif // MYWIDGET_H
