#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QPoint>

class Mythread : public QObject
{
    Q_OBJECT
public:
    explicit Mythread(QObject *parent = nullptr);

    //线程处理函数
    void drawImage();


signals:
    void updateImage(QImage temp);
public slots:
};

#endif // MYTHREAD_H
