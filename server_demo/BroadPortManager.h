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

private:
    static BroadPortManager *instance;
    explicit BroadPortManager(QObject *parent = nullptr);
    list<User> ports;

signals:

public slots:
};

#endif // BROADPORTMANAGER_H
