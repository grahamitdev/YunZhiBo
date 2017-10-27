#include "RoomManager.h"
#include "Packet.h"
RoomManager *RoomManager::instance = NULL;

RoomManager *RoomManager::getInstance()
{
    if(instance == NULL)
    {
        instance = new RoomManager();
    }
    return instance;
}



void RoomManager::addRoom(User user)
{
    users.push_back(user);
    strList.append("主播:"+user.getName()+"\t房间号:"+QString::number(user.getPort()));
}

QStringList RoomManager::getQStringList()
{
    return strList;
}

RoomManager::RoomManager(QObject *parent) : QObject(parent)
{

}
