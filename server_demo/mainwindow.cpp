#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <ClientSocket.h>
#include <QThread>
#include <SocketManager.h>
#include <QMetaType>
#include "DBHelper.h"
#include <QSqlQuery>
#include <QSqlError>

/*
 * 实现多客户端的服务器
 * 设计ClientSocket类，封装服务器socket套接字的功能onReadyRead
 * 委托thread线程处理readyRead信号和onReadyRead和emit sigWrite
 * 服务器的socket是在主线程里创建的，write要在主线程里执行，socket自身和data要(sigWrite)发射到主线程里
 * 设计SocketManager类保存所有客户端的套接字，当群发时通过SocketManager对象实现
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ip = "127.0.0.1";
    port = 8888;
    port_broadcast = 10000;
    server = new QTcpServer(this);
    sender = new QUdpSocket(this);
    SocketManager *sm = SocketManager::getInstance();
    //主线程监听群发信号
    //主线程从此刻开始监听SocketManager的sigWrite(QTcpSocket *socket,const char *data,int len)轰炸信号
    //connect(sm,SIGNAL(sigWrite(QTcpSocket*,const char*,int)),this,SLOT(onSigWrite(QTcpSocket*,const char*,int)));

    qRegisterMetaType<Pack>("Pack");


    connect(server,SIGNAL(newConnection()),this,SLOT(onNewConnection()));
    this->on_btn_listen_clicked();//开始listen套接字
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createDatabaseTable()
{
    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();
    QSqlQuery query;
    bool ret = true;
    ret = query.exec("create table if not exists tb_user("
                     "name text primary key,"
                     "passwd text not null,"
                     "question text not null,"
                     "answer text not null,"
                     "points integer default(0)"
                     ");");
    if(ret)
    {
        qDebug()<<"create tb_user success";
    }
    else
    {
        qDebug()<<"create tb_user failed:"+query.lastError().text();
    }
    helper->disConnectDatabase();
}

//主线程监听对象发射的打印log信号和log信息
void MainWindow::onPrintLog(QString log)
{
    ui->te_debug->append(log);
}
//主线程监听到按钮事件
void MainWindow::on_btn_listen_clicked()
{
    //连接数据库，创建表
    createDatabaseTable();
    //listen套接字
    if(server->isListening())
    {
        ui->te_debug->append("already listen");
        return;
    }
    if(server->listen(QHostAddress(ip),port))
    {
        QString serverInfo = "IP:"+ip+"  PORT:"+QString::number(port);
        ui->te_debug->append("listen["+serverInfo+"]success");
    }
    else
    {
        ui->te_debug->append("listen failed");
    }
}
//监听到有新的套接字连接
void MainWindow::onNewConnection()
{    
    QTcpSocket *client = server->nextPendingConnection();
    QString ip = client->peerAddress().toString();
    QString port = QString::number(client->peerPort());
    ui->te_debug->append("IP:"+ip+" PORT:"+port+" connected");//show套接字信息

    SocketManager *sm = SocketManager::getInstance();
    sm->addSocket(client);//此处获取单例对象只是为了压一个套接字进去，没有别的作用

    ClientSocket *cs = new ClientSocket(client);//socket找cs作代理

    //有了deleterLater()是否需要手动释放,没有deleterLater()是否需要手动释放
    QThread *thread = new QThread(this);
    //主线程从此刻开始监听cs对象从thread线程中发出的sigWrite(QTcpSocket*,const char*,int)信号

    connect(cs,SIGNAL(sigWriteToClient(QTcpSocket*,Pack)),this,SLOT(onSigWriteToClient(QTcpSocket*,Pack)));
    connect(cs,SIGNAL(sigSendPortByBroad(Pack)),this,SLOT(onSigSendPortByBroad(Pack)));
    connect(client,SIGNAL(disconnected()),cs,SLOT(deleteLater()));//主线程监听socket脱离信号
    connect(client,SIGNAL(disconnected()),thread,SLOT(quit()));//主线程监听socket脱离信号
    connect(client,SIGNAL(disconnected()),this,SLOT(onDisconnected()));//主线程监听socket脱离信号
    cs->moveToThread(thread);
    thread->start();

    //thread线程执行后(exec())，这个onNewConnection()也就结束了
    //但是和client、cs相关的信号主线程还是一直在监听
    //猜想：连接函数一旦执行，在本线程里就一直有效
}
//绕了个弯子：由于主线程readyRead()信号不能带参，所以无法向主线程onReadyRead()槽函数中传client对象
//主线程也就无法向客户端write数据
//所以把套接字对象封装到一个ClientSocket类中
//因为类的对象通过moveToThread(thread)可以把自己的槽函数委托给thread线程执行
//thread线程中经过连接，thread中onReadyRead()槽函数开始监听系统readyRead()信号
//thread中onReadyRead()槽里发sigWrite(QTcpSocket *socket, const char *data, int len)信号
//激发主线程中的onSigWrite(QTcpSocket *socket, const char *data, int len)槽函数
//主线程onSigWrite(QTcpSocket *socket, const char *data, int len)槽函数里client->write(data,len);

//主线程监听到写套接字信号


void MainWindow::onSigWriteToClient(QTcpSocket *socket, Pack pack)
{
    ui->te_debug->append("MainThread receive thread signal:"+ QString(pack.info));
    socket->write((char *)&pack,sizeof(Pack));
    ui->te_debug->append("MainThread has forward pack");
}

void MainWindow::onSigSendPortByBroad(Pack pack)
{
    sender->writeDatagram((char *)&pack,sizeof(Pack),QHostAddress::Broadcast,port_broadcast);
    ui->te_debug->append("send a Udp packet");
}



//主线程监听到套接字脱离信号
void MainWindow::onDisconnected()
{
    ui->te_debug->append("disconnected");
}


