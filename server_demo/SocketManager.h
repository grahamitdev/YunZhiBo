#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <vector>
using namespace std;
class SocketManager : public QObject
{
    Q_OBJECT
public:
    static SocketManager *getInstance();
    void addSocket(QTcpSocket *socket);//对外接口
    void sendDataToAll(const char *data, int len);//对外接口
signals:
    void sigWrite(QTcpSocket *socket,const char *data,int len);
public slots:

private:
    static SocketManager *instance;
    explicit SocketManager(QObject *parent = nullptr);
    vector<QTcpSocket *> sockets;

};

#endif // SOCKETMANAGER_H
