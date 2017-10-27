#include "RoomManager.h"
RoomManager *RoomManager::instance = NULL;
RoomManager *RoomManager::getInstance()
{
    if(instance==NULL)
    {
        instance = new RoomManager();
    }
    return instance;
}

void RoomManager::addRoom()
{

}

void RoomManager::addRoomMember()
{

}

RoomManager::RoomManager(QObject *parent) : QObject(parent)
{

}
