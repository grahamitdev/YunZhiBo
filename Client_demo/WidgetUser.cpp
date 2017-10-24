#include "WidgetUser.h"
#include "ui_WidgetUser.h"
#include "Packet.h"
#include "User.h"
#include <QDebug>
#include <QMessageBox>
#include "RoomManager.h"
WidgetUser::WidgetUser(QWidget *parentWidget,QWidget *parent) :
    QWidget(parent),parentWidget(parentWidget),
    ui(new Ui::WidgetUser)
{
    ui->setupUi(this);
    dialogLive = new DialogLive(this);
    connect(dialogLive,SIGNAL(sigWrite(int,User)),this,SLOT(onSigWrite(int,User)));
    //接收主界面的连接放在主机面的构造写了

    portServer = 10000;
    receiverServer = new QUdpSocket(this);
    receiverServer->bind(portServer,QUdpSocket::ReuseAddressHint);
    connect(receiverServer,SIGNAL(readyRead()),this,SLOT(onReadyReadServer()));

    widgetLook = new WidgetLook();

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

//这里不拦截关闭信号
//void WidgetUser::closeEvent(QCloseEvent *event)
//{
//    parentWidget->show();
//}

void WidgetUser::flushRoom()
{
    roomItemModel->clear();
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
        roomItemModel->appendRow(item);
    }

    ui->listView->setModel(roomItemModel);//显示model中的内容

}

void WidgetUser::on_btn_live_clicked()
{
    dialogLive->show();
    int type = TYPE_CREATEROOM;//去向服务器要port口
    QString name = QName;
    QString str = "createRoom";
    User user(type,name,str);
    sigWrite(type,user);//在主播打开摄像头之前就去要port，先建一个房间用于聊天    
}

QWidget *WidgetUser::getPerantWidget()
{
    return parentWidget;
}

void WidgetUser::onSigWrite(int type, User user)
{
    emit sigWrite(type,user);
}

void WidgetUser::onSigToWidgetUser(int type, User user)
{
    RoomManager *rm = RoomManager::getInstance();
    switch (type) {
    case TYPE_CREATEROOM:
        ui->label_2->setText("申请port口成功");//暂时放在公告里显示
        break;
    case TYPE_PORTS:
        rm->addRoom(user);//但是不知道什么时候房间信息传完
        flushRoom();
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
            break;
        case TYPE_PORTS://客户端收到新主播的name+port
            name = QString::fromLocal8Bit(pack.name);
            //QMessageBox::information(this,"直播消息",name+"开始直播了");
            //下面在客户端的房间列表里要增加一个item
            user.setName(name);
            user.setPort(port);
            rm->addRoom(user);
            flushRoom();//刷新房间列表,可用按钮激发
            break;
        default:
            break;
        }
    }
}

void WidgetUser::itemClicked(QModelIndex index)
{
    //widgetLook->show();
    isDoubleClicked++;
    if(isDoubleClicked==1)
    {
        pQTimerDoubleClicked->start(160);
    }
    if(isDoubleClicked==2)
    {
        isDoubleClicked = 0;
        pQTimerDoubleClicked->stop();
        widgetLook->show();
    }
}

void WidgetUser::itemDoubleClicked(QModelIndex index)
{
    QMessageBox::information(this,"double","QString::number(index.c)");
}

void WidgetUser::on_btn_flush_clicked()
{
    flushRoom();
}

void WidgetUser::timerSingleClick()
{
    isDoubleClicked = 0;
    pQTimerDoubleClicked->stop();
    ui->tabWidget->setCurrentIndex(1);
}
