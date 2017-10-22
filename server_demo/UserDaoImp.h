#ifndef USERDAOIMP_H
#define USERDAOIMP_H
#include "UserDao.h"

class UserDaoImp:public UserDao
{
public:
    UserDaoImp();
    virtual bool insertUser(User user);//增
    virtual bool deleteUser(QString name);//删
    virtual bool updateUser(QString name,QString passwd);//改
    virtual vector<User> selectAllUser();//查
    virtual bool selectUserByName(QString name);//按name查
    virtual bool selectUserByNameAndPasswd(QString name, QString passwd);//按name+passwd查

};

#endif // USERDAOIMP_H
