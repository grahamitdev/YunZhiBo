#ifndef DBHELPER_H
#define DBHELPER_H
#include <QSqlDatabase>

class DBHelper
{
public:
    static DBHelper *getInstance();
    void connectDatabase();
    void disConnectDatabase();
private:
    static DBHelper *instance;
    DBHelper();
    QSqlDatabase db;
};

#endif // DBHELPER_H
