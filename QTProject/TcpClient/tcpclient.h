#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>

#include <QTcpSocket>

namespace Ui {
class TcpClient;
}

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget *parent = 0);
    ~TcpClient();

private:
    Ui::TcpClient *ui;

    QTcpSocket *client;
};

#endif // TCPCLIENT_H
