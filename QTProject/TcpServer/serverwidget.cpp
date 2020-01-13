#include "serverwidget.h"
#include "ui_serverwidget.h"

ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    ui->setupUi(this);
    setWindowTitle("服务器：8888");

    tcpSocket = nullptr;

    tcpServe = new QTcpServer(this);
    tcpServe->listen(QHostAddress::Any,8888);

    //tcpSocket = new QTcpSocket(this);

    connect(tcpServe,&QTcpServer::newConnection,
            [=](){   //使用匿名函数Lamdan表达式
                    //取出建立好的套接字
                    tcpSocket = tcpServe->nextPendingConnection();
                    QString ip = tcpSocket->peerAddress().toString();
                    qint16 port = tcpSocket->peerPort();
                    QString temp = QString("[1%:2%] :成功链接").arg(ip).arg(port);
                    ui->textEditRead->setText(temp);

                    connect(tcpSocket,&QTcpSocket::readyRead,
                            [=](){
                                   //从通信套接字取出消息
                                    QByteArray array = tcpSocket->readAll();
                                    ui->textEditRead->append(array);

                                  }
                            );
                }
            );
}

ServerWidget::~ServerWidget()
{
    delete ui;
}

//向客户端发送消息
void ServerWidget::on_buttonSend_clicked()
{
    if(nullptr == tcpSocket){
        return ;
    }
    QString message = ui->textEditWrite->toPlainText();
    //给对方发送数据,触发readyRead信号
    tcpSocket->write(message.toUtf8().data());

    ui->textEditWrite->clear();
}

//关闭服务器链接
void ServerWidget::on_buttonClose_clicked()
{
    if(nullptr == tcpSocket){
        return ;
    }
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    tcpSocket = nullptr;
}
