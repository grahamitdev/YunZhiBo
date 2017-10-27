#include "SocketManager.h"
#include <QDebug>
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

void SocketManager::deleteSocket(QTcpSocket *socket)
{
    list<QTcpSocket *>::iterator it;
    for(it = sockets.begin();it!=sockets.end();++it)
    {
        if(socket==*it)
        {
            sockets.erase(it);            
            return;
        }
    }
}

void SocketManager::sendDataToAll(const char *data,int len)
{
    list<QTcpSocket *>::iterator it;
    for(it=sockets.begin();it!=sockets.end();++it)
    {
        QTcpSocket *socket = *it;
        //
    }
}

list<QTcpSocket *> SocketManager::getSockets()
{
    return sockets;
}

SocketManager::SocketManager(QObject *parent) : QObject(parent)
{

}
