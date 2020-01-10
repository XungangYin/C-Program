#ifndef NETGAME_H
#define NETGAME_H


#include "board.h"

#include <QTcpServer>
#include <QTcpSocket>

class NetGame : public Board
{
    Q_OBJECT
public:
    NetGame(bool server);

public slots:
    void slotNewConnection();
    void slotRecv();

private:
    QTcpServer * _server;
    QTcpSocket * _socket;
};

#endif // NETGAME_H
