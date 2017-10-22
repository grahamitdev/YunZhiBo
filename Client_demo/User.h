#ifndef USER_H
#define USER_H
#include <QString>

class User
{
public:
    User(QString name,QString passwd,QString question,QString answer,int points);

    User(int type,QString name,QString passwd,QString question,QString answer);
    User(int type, QString name, QString str);
    User(int type,QString info);
    User(int type,QString name,unsigned short int port);

    int getType();
    QString getName();
    QString getPasswd();
    QString getQuestion();
    QString getAnswer();
    QString getMessage();
    QString getInfo();
    unsigned short int getPort();
    int getPoints();
private:
    int type;
    QString name;//用户名
    QString passwd;//密码
    QString question;//密保
    QString answer;//答案
    QString message;
    QString info;
    unsigned short int port;
    int points;//积分
};

#endif // USER_H
