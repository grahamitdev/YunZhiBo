/*******************这个单例保存所有房间信息：房间port+当前房间的观众(name+依附的套接字)***********************/
#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H

#include <QObject>
#include <list>
#include <map>
#include "User.h"
#include "Packet.h"
using namespace std;
class RoomManager : public QObject
{
    Q_OBJECT
public:
    static RoomManager *getInstance();
    void addRoom();
    void addRoomMember();

private:
    static RoomManager *instance;
    explicit RoomManager(QObject *parent = nullptr);
    list<User> users;


signals:

public slots:
};

#endif // ROOMMANAGER_H
