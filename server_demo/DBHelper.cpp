#include "DBHelper.h"
#include <QDebug>
DBHelper *DBHelper::instance = 0;
DBHelper *DBHelper::getInstance()
{
    if(instance == NULL)
    {
        instance = new DBHelper();
    }
    return instance;
}

void DBHelper::connectDatabase()
{
    db.setDatabaseName("user.db");
    bool ret = db.open();
    if(ret)
    {
        qDebug()<<"open db success";
    }
    else
    {
        qDebug()<<"open db failed";
    }
}

void DBHelper::disConnectDatabase()
{
    db.close();
    db.removeDatabase("user.db");
    qDebug()<<"db has closed";
}

DBHelper::DBHelper()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}


















