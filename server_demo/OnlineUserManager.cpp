#include "OnlineUserManager.h"
OnlineUserManager *OnlineUserManager::instance = 0;
OnlineUserManager *OnlineUserManager::getInstance()
{
    if(instance == NULL)
    {
        instance = new OnlineUserManager();
    }
    return instance;
}

void OnlineUserManager::addOnlineUser(User user)
{
    onlineUsers.push_back(user);
}

void OnlineUserManager::deleteOnlineUser(User user)
{
    list<User>::iterator it;
    for(it = onlineUsers.begin();it!=onlineUsers.end();++it)
    {
        if(user.getName()==it->getName())
        {
            onlineUsers.erase(it);            
            return;
        }
    }
}

bool OnlineUserManager::selectOnlineUserByName(User user)
{
    list<User>::iterator it;
    for(it = onlineUsers.begin();it!=onlineUsers.end();++it)
    {
        if(user.getName()==it->getName())
        {           
            return true;
        }
    }
    return false;
}

list<User> OnlineUserManager::getOnlineUsers()
{
    return onlineUsers;
}

OnlineUserManager::OnlineUserManager(QObject *parent) : QObject(parent)
{

}
