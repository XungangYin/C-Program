#include "netgame.h"
#include <QDebug>

/* 发送消息
    1) 执红还是黑。服务器发出，客户端接受
        第一个字节固定是1，第二个字节是1或者0,1表示接收方走红，0表示走黑
    2）点击信息
        第一个字节固定是2，第二个 row，第三 col，第四 点击棋子的id
*/

NetGame::NetGame(bool server)
{
    _server = nullptr;
    _socket = nullptr;

    //若是服务器链接
    if(server){
        _server = new QTcpServer(this);
        _server->listen(QHostAddress::Any,9999);
        //新连接入的客户端
        connect(_server,SIGNAL(newConnection()),
                this,SLOT(slotNewConnection()));

    }else{   //客户端连接
        _socket = new QTcpSocket(this);
        _socket->connectToHost(QHostAddress("127.0.0.1"),9999);

        connect(_socket,SIGNAL(readyRead()),
                this,SLOT(slotRecv()));

    }
}

void NetGame::slotRecv(){

}

void NetGame::slotNewConnection(){
    if(_socket) return;
    _socket =  _server->nextPendingConnection(); //返回socket
    //qDebug()<<"客户端已连接";

    char buf[2];
    buf[0] = 1;
    buf[1] = 0;
    _socket->write(buf,2);

}
