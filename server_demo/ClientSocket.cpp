#include "ClientSocket.h"
#include <QDebug>
#include <SocketManager.h>
#include <UserDaoImp.h>
#include "User.h"
#include "Packet.h"
#include <string.h>
#include "BroadPortManager.h"


ClientSocket::ClientSocket(QTcpSocket *socket, QObject *parent)
    :QObject(parent),socket(socket)
{
    //thread线程监听这个socket套接字readyRead()信号
    connect(socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
}

void ClientSocket::regist(QString name,QString passwd,QString question,QString answer,int points)
{
    User user(name,passwd,question,answer,points);
    UserDao *ud = new UserDaoImp();
    if(ud->selectUserByName(user.getName()))
    {
        //已经注册
        Pack pack;
        pack.type = TYPE_REG_ERROR;
        strcpy(pack.info,"该用户名已经被注册，请您换一个用户名");
        emit sigWriteToClient(socket,pack);
        return;
    }
    if(ud->insertUser(user))
    {
        //插入数据库成功
        Pack pack;
        pack.type = TYPE_REG_SUCCESS;
        strcpy(pack.info,"注册成功，欢迎使用云直播");
        emit sigWriteToClient(socket,pack);
    }
    else
    {
        //插入数据库失败
        Pack pack;
        pack.type = TYPE_REG_ERROR;
        strcpy(pack.info,"对不起，服务器开销差了，请您重新注册");
        emit sigWriteToClient(socket,pack);
        return;
    }

}

void ClientSocket::login(int type, QString name, QString passwd)
{
    User user(type,name,passwd);
    UserDao *ud = new UserDaoImp();
    if(ud->selectUserByNameAndPasswd(user.getName(),user.getPasswd()))
    {
        //允许登录
        Pack pack;
        pack.type = TYPE_LOG_SUCCESS;
        strcpy(pack.info,"登录成功，欢迎使用云直播");
        emit sigWriteToClient(socket,pack);
    }
    else
    {
        //找不到这个用户或者密码错误
        Pack pack;
        pack.type = TYPE_LOG_ERROR;
        strcpy(pack.info,"用户名或密码错误，登录失败，请重新登录");
        emit sigWriteToClient(socket,pack);
        return;
    }
}
//发弹幕
void ClientSocket::barrage(int type, QString name, QString message)
{
    //发弹幕
}
//Tcp接收UDP广播的PORT口
void ClientSocket::receiveBroadPort(int type, QString name, unsigned short port)
{
    User user(type,name,port);
    BroadPortManager *bm = BroadPortManager::getInstance();
    bm->addPort(user);//保存主播广播Port

    Pack pack;
    pack.type=TYPE_PORT;
    strcpy(pack.info,"服务器已收录广播port,即将分发");
    emit sigWriteToClient(socket,pack);
    list<User> ports = bm->getPorts();
    list<User>::iterator it;
    for(it=ports.begin();it!=ports.end();++it)
    {
        pack.type = TYPE_PORTS;
        strcpy(pack.name,it->getName().toLocal8Bit().data());
        pack.port = it->getPort();
        emit sigSendPortByBroad(pack);
    }

}

void ClientSocket::onReadyRead()
{
    Pack pack;
    socket->read((char*)&pack,sizeof(Pack));
    int type = pack.type;
    QString name = QString::fromLocal8Bit(pack.name);
    QString passwd = QString::fromLocal8Bit(pack.passwd);
    QString question = QString::fromLocal8Bit(pack.question);
    QString answer = QString::fromLocal8Bit(pack.answer);
    QString message = QString::fromLocal8Bit(pack.message);
    QString info = QString::fromLocal8Bit(pack.info);
    unsigned short int port = pack.port;

    int points = 0;

    switch (type) {
    case TYPE_REG:
        regist(name,passwd,question,answer,points);
        break;
    case TYPE_LOG:
        login(type,name,passwd);
        break;
    case TYPE_MSG:
        barrage(type,name,message);
        break;
    case TYPE_PORT:
        receiveBroadPort(type,name,port);
        break;
    default:
        break;
    }




    /*
    char str[20] = {'\0'};
    socket->read(str,20);
    //socket->write(str,20);
    //错因：socket对象在主线程里，write在thread线程里
    //测试得：之后这个socket就废了，连客户端脱离都检测不到

    //emit sigWrite(socket,str,20);//如果这个套接字自己发射信号到主线程，
    //然后由主线程把数据写向客户端，可以使用

    //但是我们这里需要向连在服务器上的所有套接字发数据
    //所以此处自己不发送信号，而是调用SocketManager的接口
    //把数据传给SocketManager对象
    //SocketManager对象在sendDataToAll(const char *data,int len)里，打包
    //连番发射sigWrite(QTcpSocket *socket,const char *data,int len)信号给主线程，
    //主线程通过onSigWrite(QTcpSocket *socket,const char *data,int len)连番把数据写向客户端
    SocketManager *sm = SocketManager::getInstance();
    sm->sendDataToAll(str,20);//使用SocketManager接口
    */
}
