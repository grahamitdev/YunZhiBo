#include "User.h"
#include "Packet.h"
User::User(QString name, QString passwd, QString question, QString answer, int points)
    :name(name),passwd(passwd),question(question),answer(answer),points(points)
{

}

User::User(int type, QString name, QString passwd, QString question, QString answer)
    :type(type),name(name),passwd(passwd),question(question),answer(answer)
{

}

User::User(int type, QString name, QString str)
    :type(type),name(name)
{
    switch (type) {
    case TYPE_LOG:
        passwd = str;
        break;
    case TYPE_MSG:
        message = str;
        break;
    default:
        break;
    }
}

User::User(int type, QString info)
    :type(type),info(info)
{

}

User::User(int type, QString name, unsigned short port)
    :type(type),name(name),port(port)
{

}

int User::getType()
{
    return type;
}

QString User::getName()
{
    return name;
}

QString User::getPasswd()
{
    return passwd;
}

QString User::getQuestion()
{
    return question;
}

QString User::getAnswer()
{
    return answer;
}

QString User::getMessage()
{
    return message;
}

QString User::getInfo()
{
    return info;
}

unsigned short int User::getPort()
{
    return port;
}

int User::getPoints()
{
    return points;
}
