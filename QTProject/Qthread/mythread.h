#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = nullptr);

protected:
    //虚函数。真正的线程处理函数
    void run();

signals:
    void isDone();
public slots:
};

#endif // MYTHREAD_H
