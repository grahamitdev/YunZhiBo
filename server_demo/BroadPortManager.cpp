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



unsigned short BroadPortManager::getBroadCastPort()
{
    PORT+=1;
    return PORT;
}

void BroadPortManager::addRoom(User userRoom)//主播name+port
{
    list<User> roomMembers;//首次房间里空，没有人
    pair<User,list<User>> room(userRoom,roomMembers);
    rooms.insert(room);//存到map中了    
}

void BroadPortManager::deleteRoom(User userRoom)
{
    map<User,list<User>>::iterator it;
    for(it = rooms.begin();it!=rooms.end();++it)
    {
        if(userRoom.getPort() == it->first.getPort())//根据port查找
        {
            rooms.erase(it);            
            return;
        }
    }
}

void BroadPortManager::addRoomMember(User userRoom, User userClient)
{
     map<User,list<User>>::iterator it;
     for(it = rooms.begin();it!=rooms.end();++it)
     {
         if(userRoom.getPort() == it->first.getPort())//找到对应房间
         {             
             list<User> *roomMember = getRoomMembers(userRoom);
             roomMember->push_back(userClient);//在列表最后插入这个用户
             return;
         }
     }
}

void BroadPortManager::deleteRoomMember(User userRoom, User userClient)
{
    map<User,list<User>>::iterator it;
    for(it = rooms.begin();it!=rooms.end();++it)
    {
        if(userRoom.getPort() == it->first.getPort())//找到对应房间
        {
            list<User> *roomMember = getRoomMembers(userRoom);//获取这个房间用户列表
            list<User>::iterator it_member;
            for(it_member=roomMember->begin();it_member!=roomMember->end();++it_member)
            {                
                if(userClient.getName()==it_member->getName())//根据名字查找成员
                {
                    roomMember->erase(it_member);
                    return;
                }
            }            
            return;
        }
    }
}

list<User> BroadPortManager::getPorts()
{
    list<User> portUsers;
    map<User,list<User>>::iterator it;
    for(it = rooms.begin();it!=rooms.end();++it)
    {
        portUsers.push_back(it->first);
    }
    return portUsers;
}

list<User> *BroadPortManager::getRoomMembers(User userRoom)
{
    map<User,list<User>>::iterator it;
    for(it = rooms.begin();it!=rooms.end();++it)
    {
        if(userRoom.getPort() == it->first.getPort())//找到对应房间
        {
            return &(it->second);
        }
    }    
    return NULL;
}


BroadPortManager::BroadPortManager(QObject *parent) : QObject(parent)
{
    PORT = 10000;
}
