#ifndef USER_H
#define USER_H
#include <QString>
#include <QTcpSocket>

class User
{
public:
    User();
    User(QString name,QString passwd,QString question,QString answer,int points);

    User(int type,QString name,QString passwd,QString question,QString answer);
    User(int type, QString name, QString str);//密码,弹幕,创建房间
    User(int type,QString info);
    User(int type,QString name,unsigned short int port);
    bool operator < (const User &user)const;

    int getType();
    QString getName();
    QString getPasswd();
    QString getQuestion();
    QString getAnswer();
    QString getMessage();
    QString getInfo();
    unsigned short int getPort()const;
    int getPoints();
    QTcpSocket *getSocket();

    void setType(int);
    void setName(QString name);
    void setPasswd(QString passwd);
    void setQuestion(QString question);
    void setAnswer(QString answer);
    void setMessage(QString message);
    void setInfo(QString info);
    void setPort(unsigned short int port);
    void setPoints(int points);
    void setSocket(QTcpSocket *socket);
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
    QTcpSocket *socket;
};

#endif // USER_H
