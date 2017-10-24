#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H

#include <QObject>
#include "User.h"
#include <list>
using namespace std;
class RoomManager : public QObject
{
    Q_OBJECT
public:
    static RoomManager *getInstance();
    void addRoom(User user);
    QStringList getQStringList();
private:
    static RoomManager *instance;
    explicit RoomManager(QObject *parent = nullptr);
    list<User> users;
    QStringList strList;

signals:

public slots:
};

#endif // ROOMMANAGER_H
