/*********************这个客户端保存在线用户名+依附的客户端套接字，可用于统计当前在线的用户总数***********************/
#ifndef ONLINEUSERMANAGER_H
#define ONLINEUSERMANAGER_H

#include <QObject>
#include "User.h"
#include "list"
using namespace std;
class OnlineUserManager : public QObject
{
    Q_OBJECT
public:
    static OnlineUserManager *getInstance();
    void addOnlineUser(User user);
    void deleteOnlineUser(User user);
    bool selectOnlineUserByName(User user);

    list<User> getOnlineUsers();

private:
    static OnlineUserManager *instance;
    explicit OnlineUserManager(QObject *parent = nullptr);
    list<User> onlineUsers;
signals:

public slots:
};

#endif // ONLINEUSERMANAGER_H
