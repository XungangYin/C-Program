#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QTcpServer>  //监听套接字
#include <QTcpSocket> //通信套接字

#include <QWidget>

namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = 0);
    ~ServerWidget();

private slots:
    void on_buttonSend_clicked();

    void on_buttonClose_clicked();

private:
    Ui::ServerWidget *ui;

    QTcpServer *tcpServe ;
    QTcpSocket *tcpSocket;
};

#endif // SERVERWIDGET_H
