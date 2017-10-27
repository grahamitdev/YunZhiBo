#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "User.h"
#include "Packet.h"
//套接字的代理类
//增加更多的接口

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QTcpSocket *socket, QObject *parent = nullptr);
    void regist(QString name,QString passwd,QString question,QString answer,int points);
    void login(int type,QString name,QString passwd);
    void barrage(int type,QString name,QString message,unsigned short int port);
    void changePasswd(int type,QString,QString passwd,QString question,QString answer);
    void goBackPasswd(int type,QString,QString passwd,QString question,QString answer);
    void receiveBroadPort(int type,QString,unsigned short int port);
    void createRoom(int type,QString name,QString info);
    void askForPorts(int type);
    void disConnect(int type);
    void logOff(int type,QString name);
    void inRoom(int type,QString name,unsigned short int port);
    void outRoom(int type,QString name,unsigned short int port);
    void sendRoomMember(int type,unsigned short int port);


signals:    
    void sigWriteToClient(QTcpSocket *socket,Pack pack);
    void sigSendPortByBroad(Pack pack);//全局广播
    void changeNumSocket(int);
    void changeNumUser(int);
    void changeNumRoom(int);
public slots:
    void onReadyRead();
    void onDisConnected();
private:
    QTcpSocket *socket;
};

#endif // CLIENTSOCKET_H
