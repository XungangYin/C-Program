#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QHostAddress>

TcpClient::TcpClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    client = new QTcpSocket(this);

    connect(client,&QTcpSocket::connected,
            [=](){
                    ui->textEditRead->setText("成功链接");
                }
            );


    //从服务器接受数据
    connect(client,&QTcpSocket::readyRead,
            [=](){
                    QByteArray mess = client->readAll();
                    ui->textEditRead->append(mess);
                 }
    );
}

TcpClient::~TcpClient()
{
    delete ui;
}



void TcpClient::on_connectToServer_clicked()
{
    //获取服务器的IP和端口号
    qint16 port  =ui->lineEditPort->text().toInt();
    QString ip = ui->lineEditIP->text();

    //主动和服务器建立链接
    client->connectToHost(QHostAddress(ip),port);
}

//向服务器发送数据
void TcpClient::on_buttonSend_clicked()
{
    QString mess = ui->textEditWrite->toPlainText();

    client->write( mess.toUtf8().data() );

    ui->textEditWrite->clear();
}

void TcpClient::on_buttonClose_clicked()
{
    //主动断开链接
    client->disconnectFromHost();
    client->close();
}
