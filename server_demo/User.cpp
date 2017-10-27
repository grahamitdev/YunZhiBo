#include "User.h"
#include "Packet.h"
User::User()
{

}

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
    case TYPE_CREATEROOM:
        info = str;
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

bool User::operator <(const User &user) const
{
    return this->getPort()<user.getPort();
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

unsigned short int User::getPort() const
{
    return port;
}

int User::getPoints()
{
    return points;
}

QTcpSocket *User::getSocket()
{
    return socket;
}

void User::setType(int type)
{
    this->type = type;
}

void User::setName(QString name)
{
    this->name = name;
}

void User::setPasswd(QString passwd)
{
    this->passwd = passwd;
}

void User::setQuestion(QString question)
{
    this->question = question;
}

void User::setAnswer(QString answer)
{
    this->answer = answer;
}

void User::setMessage(QString message)
{
    this->message = message;
}

void User::setInfo(QString info)
{
    this->info = info;
}

void User::setPort(unsigned short int port)
{
    this -> port = port;
}

void User::setPoints(int points)
{
    this->points = points;
}

void User::setSocket(QTcpSocket *socket)
{
    this->socket = socket;
}


