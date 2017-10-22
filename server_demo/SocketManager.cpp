#include "SocketManager.h"
SocketManager *SocketManager::instance = NULL;
SocketManager *SocketManager::getInstance()
{
    if(instance == NULL)
    {
        instance = new SocketManager;
    }
    return instance;
}

void SocketManager::addSocket(QTcpSocket *socket)
{
    sockets.push_back(socket);
}

void SocketManager::sendDataToAll(const char *data,int len)
{
    vector<QTcpSocket *>::iterator it;
    for(it=sockets.begin();it!=sockets.end();++it)
    {
        QTcpSocket *socket = *it;
        emit sigWrite(socket,data,len);
    }
}

SocketManager::SocketManager(QObject *parent) : QObject(parent)
{

}
