#ifndef USERDAO_H
#define USERDAO_H
#include "User.h"
#include <vector>
using namespace std;
class UserDao
{
public:
    UserDao();
    virtual bool insertUser(User user) = 0;//增
    virtual bool deleteUser(QString name) = 0;//删
    virtual bool updateUser(QString name,QString passwd) = 0;//改
    virtual vector<User> selectAllUser() = 0;//查
    virtual bool selectUserByName(QString name) = 0;//按name查
    virtual bool selectUserByNameAndPasswd(QString name,QString passwd) = 0;//按passwd查

    virtual ~UserDao();
};

#endif // USERDAO_H
