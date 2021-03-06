/*****************这个单例只保存客户端套接字，没有用户信息,可用于统计当前连接的客户端设备数********************/
#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <list>
using namespace std;
class SocketManager : public QObject
{
    Q_OBJECT
public:
    static SocketManager *getInstance();
    void addSocket(QTcpSocket *socket);//对外接口
    void deleteSocket(QTcpSocket *socket);
    void sendDataToAll(const char *data, int len);//对外接口
    list<QTcpSocket *> getSockets();
signals:
    void sigWrite(QTcpSocket *socket,const char *data,int len);
public slots:

private:
    static SocketManager *instance;
    explicit SocketManager(QObject *parent = nullptr);
    list<QTcpSocket *> sockets;

};

#endif // SOCKETMANAGER_H
