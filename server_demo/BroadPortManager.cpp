#include "BroadPortManager.h"

BroadPortManager *BroadPortManager::instance = NULL;

BroadPortManager *BroadPortManager::getInstance()
{
    if(instance == NULL)
    {
        instance = new BroadPortManager();
    }
    return instance;
}

list<User> BroadPortManager::getPorts()
{
    return ports;
}

void BroadPortManager::addPort(User user)
{
    ports.push_back(user);
}

unsigned short BroadPortManager::getBroadCastPort()
{
    PORT+=1;
    return PORT;
}

list<User> BroadPortManager::getBroadCastPorts()
{
    return ports;
}

BroadPortManager::BroadPortManager(QObject *parent) : QObject(parent)
{
    PORT = 10001;
}
