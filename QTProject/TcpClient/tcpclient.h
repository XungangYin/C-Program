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

private slots:
    void on_connectToServer_clicked();

    void on_buttonSend_clicked();

    void on_buttonClose_clicked();

private:
    Ui::TcpClient *ui;

    QTcpSocket *client;
};

#endif // TCPCLIENT_H
