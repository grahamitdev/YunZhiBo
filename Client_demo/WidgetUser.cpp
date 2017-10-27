#include "WidgetUser.h"
#include "ui_WidgetUser.h"
#include "Packet.h"
#include "User.h"
#include <QDebug>
#include <QMessageBox>
#include "RoomManager.h"
#include <string.h>
WidgetUser::WidgetUser(QWidget *parentWidget,QWidget *parent) :
    QWidget(parent),parentWidget(parentWidget),
    ui(new Ui::WidgetUser)
{
    ui->setupUi(this);

    //接收主界面的连接放在主界面的构造写了

    portServer = 10000;
    receiverServer = new QUdpSocket(this);
    receiverServer->bind(portServer,QUdpSocket::ReuseAddressHint);
    connect(receiverServer,SIGNAL(readyRead()),this,SLOT(onReadyReadServer()));



    ui->tabWidget->setCurrentIndex(0);
    isDoubleClicked = 0;//初始化不点击
    pQTimerDoubleClicked = new QTimer(this);
    connect(pQTimerDoubleClicked,SIGNAL(timeout()),
            this,SLOT(timerSingleClick()));//关联定时器和槽函数
    roomItemModel = new QStandardItemModel(ui->listView);
    connect(ui->listView,SIGNAL(clicked(QModelIndex)),this,SLOT(itemClicked(QModelIndex)));
    //connect(ui->listView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(itemDoubleClicked(QModelIndex)));




}

WidgetUser::~WidgetUser()
{
    delete ui;
    delete widgetLook;    
}

void WidgetUser::setName(QString name)
{
    this -> QName = name;
}

void WidgetUser::closeEvent(QCloseEvent *event)
{
    parentWidget->show();
    User user;
    int type = 0;
    //下线请求
    type = TYPE_LOGOFF;
    user.setType(type);
    user.setName(QName);
    emit sigWrite(type,user);

//    //断开套接字请求
//    type = TYPE_DISCONN;
//    user.setType(type);
//    emit sigWrite(type,user);
}



//这里不拦截关闭信号
//void WidgetUser::closeEvent(QCloseEvent *event)
//{
//    parentWidget->show();
//}

void WidgetUser::flushRoom()
{
    roomItemModel->clear();//清除上次列表
    RoomManager *rm = RoomManager::getInstance();
    QStringList strList = rm->getQStringList();
    int nCount = strList.size();
    ui->lb_room_num->setText(QString::number(nCount));

    int i = 0;
    for(i = 0; i < nCount; i++)
    {
        QString string = static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);        
        if(i % 2 == 1)//交替
        {
            QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
            linearGrad.setColorAt(0, Qt::white);
            linearGrad.setColorAt(1, Qt::red);
            QBrush brush(linearGrad);
            item->setBackground(brush);
        }
        QPixmap pixmap(":/new/prefix1/logo_R.ico");//设定图片
        item->setIcon(pixmap);
        roomItemModel->appendRow(item);//重新装载
    }

    ui->listView->setModel(roomItemModel);//显示model中的内容
}

void *WidgetUser::getParentWidget()
{
    return parentWidget;
}
//建房间进入直播界面，准备开摄像头
void WidgetUser::on_btn_live_clicked()
{        
    int type = TYPE_CREATEROOM;//去向服务器要port口
    QString name = QName;
    QString str = "createRoom";
    User user(type,name,str);
    sigWrite(type,user);
    //现在只知道主播名，还不知道申请到的port

}

QWidget *WidgetUser::getPerantWidget()
{
    return parentWidget;
}

void WidgetUser::onSigWrite(int type, User user)
{
    qDebug()<<user.getMessage();
    emit sigWrite(type,user);
}

void WidgetUser::onSigToWidgetUser(int type, User user)
{
    RoomManager *rm = RoomManager::getInstance();
    switch (type) {
    case TYPE_CREATEROOM:
        ui->label_2->setText("申请port口成功:"+QString::number(user.getPort()));//暂时放在公告里显示
        port = user.getPort();
        dialogLive = new DialogLive(QName,QString::number(port),this);
        dialogLive->show();//进入直播界面
        break;
    case TYPE_PORTS:
        //作为用户登录成功后，去服务器要房间信息
        //也只要这一次
        rm->addRoom(user);//但是不知道什么时候房间信息传完
        flushRoom();//所以收到一个包就刷新一次，在界面上产生了动态的效果
        break;
    case TYPE_MSG:
        emit sigToWidgetLook(type,user);
        break;
    case TYPE_ROOMMEMBER://向widgetLook发送房间成员
        emit sigToWidgetLook(type,user);
        break;
    default:
        break;
    }
}
//接收服务器大广播转发的port/message
void WidgetUser::onReadyReadServer()
{
    //Pack pack;
    while(receiverServer->hasPendingDatagrams())//有等待数据包
    {
        Pack pack;
        receiverServer->readDatagram((char *)&pack,sizeof(Pack));
        int type = pack.type;
        QString name;
        QString message;
        unsigned short int port = pack.port;
        RoomManager *rm = RoomManager::getInstance();
        User user;

        switch (type) {
        case TYPE_MSG:
            name = QString::fromLocal8Bit(pack.name);
            message = QString::fromLocal8Bit(pack.message);
            ui->label_2->setText(message);
            QMessageBox::information(this,"服务器公告",message);
            break;
        case TYPE_PORTS://客户端收到新主播的name+port
            name = QString::fromLocal8Bit(pack.name);
            //QMessageBox::information(this,"直播消息",name+"开始直播了");
            //下面在客户端的房间列表里要增加一个item
            user.setName(name);
            user.setPort(port);
            rm->addRoom(user);//向本地房间列表中存入一个新房间信息
            flushRoom();//刷新房间列表,可用按钮激发
            break;
        default:
            break;
        }
    }
}
//用户双击进入观看界面
void WidgetUser::itemClicked(QModelIndex index)
{
    //widgetLook->show();
    isDoubleClicked++;
    if(isDoubleClicked==1)//被点击一次，等260ms
    {
        pQTimerDoubleClicked->start(260);
    }
    if(isDoubleClicked==2)//被点击两次，直接执行双击后的事件
    {
        isDoubleClicked = 0;
        pQTimerDoubleClicked->stop();
        //现在要从item中解析出主播名+房间号(port)        
        char roomInfomation[50] = {'\0'};
        strcpy(roomInfomation,index.data().toString().toStdString().data());

        int i = 0;
        int j = 0;
        int len = strlen(roomInfomation);
        char roomName[20] = {'\0'};
        char roomNum[6] = {'\0'};
        //解析房间号(port)
        for(i = len-1,j = 4;i>len-1-6;--i,--j)
        {
            roomNum[j] = roomInfomation[i];
        }
        //解析主播名
        for(i = 0;i<len;++i)
        {
            if(roomInfomation[i]!=':')
            {
                continue;
            }
            else
            {
                j = 0;
                for(i = i+1;i<len;++i)
                {
                    if(roomInfomation[i]=='\t')
                    {
                        break;
                    }
                    roomName[j++] = roomInfomation[i];
                }
                break;
            }
        }
        QString name(roomName);
        QString num(roomNum);
        //解析完毕
        //下面创建直播界面
        this->hide();//隐藏WidgetUser
        widgetLook = new WidgetLook(this,name,num,QName);
        connect(this,SIGNAL(sigToWidgetLook(int,User)),widgetLook,SLOT(onSigToWidgetLook(int,User)));
        widgetLook->show();
        //进入观看界面，目前还在主线程里，所以同一时间只能看一个房间的主播

        //下面要把这个房间的port和自己的name发给服务器
        //让服务器把自己加到这个房间的用户里
        User user;
        int type = TYPE_INROOM;
        user.setType(type);
        user.setName(QName);//观众名字
        unsigned short int port = num.toUShort();
        user.setPort(port);//房间port        
        emit sigWrite(type,user);       
    }
}


//刷新房间列表，目前没有发指令去向服务器要数据
//只是用本地数据刷新
void WidgetUser::on_btn_flush_clicked()
{
    flushRoom();
}
//单击事件，展示主播信息界面
void WidgetUser::timerSingleClick()
{
    isDoubleClicked = 0;
    pQTimerDoubleClicked->stop();
    ui->tabWidget->setCurrentIndex(1);
}
