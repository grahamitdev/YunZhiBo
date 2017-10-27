/***********这个类里的所有方法都在thread线程里执行，不影响其他客户端*********/
#include "ClientSocket.h"
#include <QDebug>
#include <SocketManager.h>
#include <UserDaoImp.h>
#include "User.h"
#include "Packet.h"
#include <string.h>
#include "BroadPortManager.h"
#include <QThread>
#include <QHostAddress>
#include "OnlineUserManager.h"
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
        QString returnInfo = "该用户名已经被注册，请您换一个用户名";
        strcpy(pack.info,returnInfo.toLocal8Bit().data());
        emit sigWriteToClient(socket,pack);
        return;
    }
    if(ud->insertUser(user))
    {
        //插入数据库成功
        Pack pack;
        pack.type = TYPE_REG_SUCCESS;
        QString returnInfo = name+":注册成功，欢迎使用云直播";
        strcpy(pack.info,returnInfo.toLocal8Bit().data());
        emit sigWriteToClient(socket,pack);
    }
    else
    {
        //插入数据库失败
        Pack pack;
        pack.type = TYPE_REG_ERROR;
        QString returnInfo = "对不起，服务器开销差了，请您重新注册";
        strcpy(pack.info,returnInfo.toLocal8Bit().data());
        emit sigWriteToClient(socket,pack);
        return;
    }

}

void ClientSocket::login(int type, QString name, QString passwd)
{
    User user(type,name,passwd);
    //先检查在线列表(少)
    OnlineUserManager *om = OnlineUserManager::getInstance();
    if(om->selectOnlineUserByName(user))
    {
        Pack pack;
        pack.type = TYPE_LOG_ERROR;
        QString returnInfo = name+"您已登录，请不要重复登录";
        strcpy(pack.info,returnInfo.toLocal8Bit().data());
        emit sigWriteToClient(socket,pack);
        return;
    }
    //再检查数据库(多)，这样减小服务器负担
    UserDao *ud = new UserDaoImp();
    if(ud->selectUserByNameAndPasswd(user.getName(),user.getPasswd()))
    {
        user.setSocket(socket);
        //附加绑定这个用户的套接字,保存到所有在线用户列表，可用于Tcp发送公告(全局消息)
        OnlineUserManager *om = OnlineUserManager::getInstance();
        om->addOnlineUser(user);
        //允许登录
        Pack pack;
        pack.type = TYPE_LOG_SUCCESS;
        QString returnInfo = name+":登录成功，欢迎使用云直播";
        strcpy(pack.info,returnInfo.toLocal8Bit().data());
        emit sigWriteToClient(socket,pack);
        emit changeNumUser(1);
    }
    else
    {
        //找不到这个用户或者密码错误
        Pack pack;
        pack.type = TYPE_LOG_ERROR;
        QString returnInfo = "用户名或密码错误，登录失败，请重新登录";
        strcpy(pack.info,returnInfo.toLocal8Bit().data());
        emit sigWriteToClient(socket,pack);
        return;
    }
}
//发弹幕
void ClientSocket::barrage(int type, QString name, QString message,unsigned short int port)
{
    User user;
    user.setType(type);
    user.setPort(port);

    Pack pack;
    QString returnInfo = "message";
    pack.type = type;
    strcpy(pack.name,name.toLocal8Bit().data());
    strcpy(pack.message,message.toLocal8Bit().data());
    strcpy(pack.info,returnInfo.toLocal8Bit().data());

    //此处先用Tcp发送给同房间的用户(在线)
    BroadPortManager *bm = BroadPortManager::getInstance();
    list<User> *roomUser = bm->getRoomMembers(user);
    list<User>::iterator it;
    for(it=roomUser->begin();it!=roomUser->end();++it)
    {
        //这里就不发给主线程去发了，为了减轻主线程负担,但是编译没报错，运行是报错
        //QSocketNotifier: Socket notifiers cannot be enabled or disabled from another thread
        //所以还是让主线程转发客户端的消息
        emit sigWriteToClient(it->getSocket(),pack);
    }

}
//Tcp接收PORT口，废弃
void ClientSocket::receiveBroadPort(int type, QString name, unsigned short port)
{
//    User user(type,name,port);
//    BroadPortManager *bm = BroadPortManager::getInstance();
//    bm->addPort(user);//保存主播广播Port

//    Pack pack;
//    pack.type=TYPE_PORT;
//    QString returnInfo = "服务器已收录广播port,即将分发";
//    strcpy(pack.info,returnInfo.toLocal8Bit().data());
//    emit sigWriteToClient(socket,pack);//Tcp

//    list<User> ports = bm->getPorts();
//    list<User>::iterator it;
//    for(it=ports.begin();it!=ports.end();++it)
//    {
//        pack.type = TYPE_PORTS;
//        strcpy(pack.name,it->getName().toLocal8Bit().data());
//        pack.port = it->getPort();
//        emit sigSendPortByBroad(pack);
//    }

}
//为主播分配一个唯一的广播Port,保存到list容器中
void ClientSocket::createRoom(int type, QString name, QString info)//info:createRooom
{
    //通过算法产生一个唯一的port口
    //下面去单例类BroadPortManager里写一个这样的算法
    BroadPortManager *bm = BroadPortManager::getInstance();
    User user(type,name,info);//name+createRoom
    user.setPort(bm->getBroadCastPort());//type+name+createRoom+port
    bm->addRoom(user);//在服务器里产生一个房间(port)
    //主播也需要把自己加进房间用户列表
    emit changeNumRoom(1);



    //先给主播一个答复,让主线程去发
    Pack pack;
    pack.type = TYPE_CREATEROOM;
    pack.port = user.getPort();
    QString port_str = QString::number(pack.port);
    QString returnInfo = "服务器已分配一个广播port:"+port_str+",即将分发给其他用户";
    strcpy(pack.info,returnInfo.toLocal8Bit().data());
    emit sigWriteToClient(socket,pack);
    inRoom(type,name,user.getPort());

    //当有新的主播开播的时候，服务器要把这个主播name+port发出去
    //目前采用Ucp广播分发给同一网段里的用户(包括新开直播的主播自己也会收到这个广播)
    //或者Tcp:轮发给当前在线的所有用户(包括这个主播自己)，但是比较耗时
    //发射给主线程去发大广播
    pack.type = TYPE_PORTS;
    strcpy(pack.name,name.toLocal8Bit().data());    
    pack.port = user.getPort();//只发新主播给整个局域网
    emit sigSendPortByBroad(pack);

}
//新观众上线，回复新观众的请求
void ClientSocket::askForPorts(int type)
{
    Pack pack;
    pack.type = type;
    User user;
    QString returnInfo;
    BroadPortManager *bm = BroadPortManager::getInstance();
    list<User> ports = bm->getPorts();//获取当前所有房间信息(port+主播名字)
    list<User>::iterator it;
    //轮循把服务器里的主播房间信息Tcp发给新登录成功的客户端,这里比较费时
    for(it=ports.begin();it!=ports.end();++it)
    {
        user = *it;
        strcpy(pack.name,user.getName().toLocal8Bit().data());
        pack.port = user.getPort();
        returnInfo = "Tcp-房间信息";
        strcpy(pack.info,returnInfo.toLocal8Bit().data());
        QThread::msleep(100);
        emit sigWriteToClient(socket,pack);
    }
}

void ClientSocket::disConnect(int type)
{
    SocketManager *sm = SocketManager::getInstance();
    sm->deleteSocket(socket);

    Pack pack;
    pack.type = type;
    QString ip = socket->peerAddress().toString();
    QString port = QString::number(socket->peerPort());
    QString returnInfo = "IP:"+ip+" PORT:"+port+" disconnected";
    strcpy(pack.info,returnInfo.toLocal8Bit().data());
    emit sigWriteToClient(socket,pack);
    emit changeNumSocket(-1);
}

void ClientSocket::logOff(int type, QString name)
{
    User user;
    user.setType(type);
    user.setName(name);
    user.setSocket(socket);
    OnlineUserManager *om = OnlineUserManager::getInstance();
    om->deleteOnlineUser(user);
    emit changeNumUser(-1);

    Pack pack;
    pack.type = type;
    QString ip = socket->peerAddress().toString();
    QString port = QString::number(socket->peerPort());
    QString returnInfo = "IP:"+ip+" PORT:"+port+",用户"+name+"请求下线成功";
    strcpy(pack.info,returnInfo.toLocal8Bit().data());
    emit sigWriteToClient(socket,pack);
}
//将用户加入对应房间列表
//目标房间port
//观众名+观众依附的socket
void ClientSocket::inRoom(int type, QString name, unsigned short port)
{
    BroadPortManager *bm = BroadPortManager::getInstance();
    User userRoom;
    userRoom.setPort(port);

    User userClient;
    userClient.setName(name);
    userClient.setSocket(socket);//绑定观众socket
    bm->addRoomMember(userRoom,userClient);

    Pack pack;
    pack.type = type;
    QString ip = socket->peerAddress().toString();
    QString QPort = QString::number(port);
    QString returnInfo = "IP:"+ip+" PORT:"+QPort+",用户"+name+"加入房间:"+QPort+"成功";
    strcpy(pack.info,returnInfo.toLocal8Bit().data());
    emit sigWriteToClient(socket,pack);


    //发送房间成员列表
    sendRoomMember(TYPE_ROOMMEMBER,port);
}
//将用户移除对应房间列表
//观众名+离开房间port
void ClientSocket::outRoom(int type, QString name, unsigned short port)
{
    BroadPortManager *bm = BroadPortManager::getInstance();
    User userRoom;
    userRoom.setPort(port);

    User userClient;
    userClient.setName(name);
    bm->deleteRoomMember(userRoom,userClient);

    Pack pack;
    pack.type = type;
    QString ip = socket->peerAddress().toString();
    QString QPort = QString::number(port);
    QString returnInfo = "IP:"+ip+" PORT:"+QPort+",用户"+name+"退出房间:"+QPort+"成功";
    strcpy(pack.info,returnInfo.toLocal8Bit().data());
    emit sigWriteToClient(socket,pack);
    //发送房间成员列表
    sendRoomMember(TYPE_ROOMMEMBER,port);
}

void ClientSocket::sendRoomMember(int type, unsigned short port)//房间port
{
    //发送房间成员列表
    BroadPortManager *bm = BroadPortManager::getInstance();
    User userRoom;
    userRoom.setPort(port);
    Pack pack;
    pack.type = type;
    User user;

    list<User> *roomMember = bm->getRoomMembers(userRoom);//获取当前所有房间信息(port+主播名字)
    list<User>::iterator it;
    for(it=roomMember->begin();it!=roomMember->end();++it)
    {
        user = *it;
        strcpy(pack.name,user.getName().toLocal8Bit().data());
        QString returnInfo = "Tcp-房间成员信息";
        strcpy(pack.info,returnInfo.toLocal8Bit().data());
        QThread::msleep(100);
        emit sigWriteToClient(socket,pack);
    }
}


//收客户端数据包
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
        barrage(type,name,message,port);
        break;
    case TYPE_PORT:
        receiveBroadPort(type,name,port);
        break;
    case TYPE_CREATEROOM:        
        createRoom(type,name,info);
        break;
    case TYPE_PORTS:
        askForPorts(type);
        break;
    case TYPE_DISCONN:
        disConnect(type);
        break;
    case TYPE_LOGOFF:
        logOff(type,name);
        break;
    case TYPE_INROOM:
        inRoom(type,name,port);
        break;
    case TYPE_OUTROOM:
        outRoom(type,name,port);
        break;
    case TYPE_ROOMMEMBER:

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
//废弃
void ClientSocket::onDisConnected()
{
    //list支持快速插入删除
    //vector支持快速访问
    SocketManager *sm = SocketManager::getInstance();
    list<QTcpSocket *> sockets = sm->getSockets();
    list<QTcpSocket *>::iterator it;
    for(it=sockets.begin();it!=sockets.end();++it)
    {
        if(socket==*it)
        {            
            sockets.erase(it);
            return;
        }
    }
}
