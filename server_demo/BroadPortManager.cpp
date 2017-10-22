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

BroadPortManager::BroadPortManager(QObject *parent) : QObject(parent)
{

}
