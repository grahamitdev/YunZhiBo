/**********************这个单例保存所有直播信息，主播名+port，可用于统计当前房间总数***********************/
#ifndef BROADPORTMANAGER_H
#define BROADPORTMANAGER_H

#include <QObject>
#include "User.h"
#include <list>
#include <map>

using namespace std;




class BroadPortManager : public QObject
{
    Q_OBJECT
public:
    static BroadPortManager *getInstance();    
    unsigned short int getBroadCastPort();//每个新开的房间都要在这里申请分配一个port
    void addRoom(User userRoom);//生成新房间
    void deleteRoom(User userRoom);//删除房间(包括房间成员全部丢弃，或者给每个房间用户发关闭房间通知)
    void addRoomMember(User userRoom, User userClient);//向房间中添加用户:name+依附的套接字
    void deleteRoomMember(User userRoom, User userClient);//从房间中删除用户
    list<User> getPorts();//对外提供房间ports接口
    list<User> *getRoomMembers(User userRoom);//对外提供房间成员接口

private:
    static BroadPortManager *instance;
    explicit BroadPortManager(QObject *parent = nullptr);
    unsigned short int PORT;//存储port口已经用到多少了
    map<User,list<User>> rooms;//存储房间(房间用户)






signals:

public slots:
};

#endif // BROADPORTMANAGER_H
