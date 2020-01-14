#include "mythread.h"
#include <QPainter>
#include <QPen>
#include <QBrush>


Mythread::Mythread(QObject *parent) : QObject(parent)
{

}
void Mythread::drawImage(){
    QImage image(500,500,QImage::Format_ARGB32);

    //定义画家，指定绘图设备
    QPainter p (&image);
    //定义画笔
    QPen pen;
    pen.setWidth(5);
    //把画笔交给画家
    p.setPen(pen);
    //定义画刷
    QBrush b;
    b.setStyle(Qt::SolidPattern); //样式
    b.setColor(Qt::red); //颜色
    //画刷交给画笔
    p.setBrush(b);
    //定义五个点
    QPoint a[] =
    {
        QPoint(qrand()%500,qrand()%500),
        QPoint(qrand()%500,qrand()%500),
        QPoint(qrand()%500,qrand()%500),
        QPoint(qrand()%500,qrand()%500),
        QPoint(qrand()%500,qrand()%500)
    };

    p.drawPolygon(a,5);


    //通过信号发送图片
    emit updateImage(image);
}
