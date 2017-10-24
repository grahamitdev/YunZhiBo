#ifndef BROADPORTMANAGER_H
#define BROADPORTMANAGER_H

#include <QObject>
#include "User.h"
#include <list>
using namespace std;
class BroadPortManager : public QObject
{
    Q_OBJECT
public:
    static BroadPortManager *getInstance();
    list<User> getPorts();
    void addPort(User user);
    unsigned short int getBroadCastPort();
    list<User> getBroadCastPorts();

private:
    static BroadPortManager *instance;
    explicit BroadPortManager(QObject *parent = nullptr);
    list<User> ports;//临时存储主播名+广播port口
    unsigned short int PORT;

signals:

public slots:
};

#endif // BROADPORTMANAGER_H
