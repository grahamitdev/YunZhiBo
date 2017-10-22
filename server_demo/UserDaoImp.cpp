#include "UserDaoImp.h"
#include "DBHelper.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
UserDaoImp::UserDaoImp()
    :UserDao()
{

}
//插入用户注册信息到tb_user
bool UserDaoImp::insertUser(User user)
{
    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();
    QSqlQuery query;
    bool ret = true;

    query.prepare("insert into tb_user(name,passwd,question,answer,points)"
                  "values(:name,:passwd,:question,:answer,:points);");
    query.bindValue(":name",user.getName());
    query.bindValue(":passwd",user.getPasswd());
    query.bindValue(":question",user.getQuestion());
    query.bindValue(":answer",user.getAnswer());
    query.bindValue(":points",user.getPoints());
    ret = query.exec();
    if(ret)
    {
        qDebug()<<"insert tb_user success";
    }
    else
    {
        qDebug()<<"insert tb_user failed:"+query.lastError().text();
    }
    helper->disConnectDatabase();
    return ret;
}
//从tb_user中删除一个用户信息
bool UserDaoImp::deleteUser(QString name)
{
    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();
    QSqlQuery query;
    bool ret = true;

    query.prepare("delete * from tb_user where name = :name;");
    query.bindValue(":name",name);
    ret = query.exec();
    if(ret)
    {
        qDebug()<<"delete one info success";
    }
    else
    {
        qDebug()<<"delete one info failed:"+query.lastError().text();
    }
    helper->disConnectDatabase();
    return ret;
}
//根据用户名修改一个用户密码
bool UserDaoImp::updateUser(QString name,QString passwd)
{
    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();
    QSqlQuery query;
    bool ret = true;

    //selectUserByName(name);
    query.prepare("select * from tb_user where name = :name;");
    query.bindValue(":name",name);
    ret = query.exec();//执行select
    if(query.next())//找到
    {
        qDebug()<<"find this info";
        query.prepare("update tb_user"
                      "set passwd = :passwd"
                      "where name = :name;");
        query.bindValue(":passwd",passwd);
        query.bindValue(":name",name);
        ret = query.exec();//执行update
        if(ret)
        {
            qDebug()<<"update success";
            ret = true;
        }
        else
        {
            qDebug()<<"update failed";
            ret = false;
        }
    }
    else
    {
        ret = false;
    }


    helper->disConnectDatabase();
    return ret;
}
//查询所有用户数据
vector<User> UserDaoImp::selectAllUser()
{
    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();
    vector<User> users;
    QSqlQuery query;
    bool ret = true;

    ret = query.exec("select * from tb_user;");
    if(ret)
    {
        while(query.next())
        {
            QString name = query.value(0).toString();
            QString passwd = query.value(1).toString();
            QString question = query.value(2).toString();
            QString answer = query.value(3).toString();
            int points = query.value(4).toInt();
            User user(name,passwd,question,answer,points);
            users.push_back(user);
        }
    }
    else
    {
        qDebug()<<"select all info failed:"+query.lastError().text();
        //如果select all 失败这里应该返回一个的东西给主调函数
    }


    helper->disConnectDatabase();
    return users;
}
//按name查
bool UserDaoImp::selectUserByName(QString name)
{
    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();
    QSqlQuery query;
    bool ret = true;

    query.prepare("select * from tb_user where name = :name;");
    query.bindValue(":name",name);
    ret = query.exec();//执行select
    if(query.next())
    {
        qDebug()<<"find";
        ret = true;
    }
    else
    {
        qDebug()<<"not find";
        ret = false;
    }

    helper->disConnectDatabase();
    return ret;
}
//按name+passwd查
bool UserDaoImp::selectUserByNameAndPasswd(QString name,QString passwd)
{
    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();
    QSqlQuery query;
    bool ret = true;

    query.prepare("select * from tb_user where name = :name and passwd = :passwd;");
    query.bindValue(":name",name);
    query.bindValue(":passwd",passwd);
    ret = query.exec();

    if(query.next())
    {
        qDebug()<<"find";
        ret = true;
    }
    else
    {
        qDebug()<<"not find";
        ret = false;
    }

    helper->disConnectDatabase();
    return ret;
}
