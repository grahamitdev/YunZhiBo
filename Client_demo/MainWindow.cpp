#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QHostAddress>
#include <QMessageBox>
#include <string.h>
#include <QDebug>
#include <string.h>
#include "Packet.h"
#include <QRegExp>
#include <QRegExpValidator>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    //用户名正则
    QRegExp regExpName("[0-9A-Za-z]{10}");//输入限定，即正则表达式，括号内的字符串是允许输入的内容
    QRegExpValidator* validatorName = new QRegExpValidator(regExpName,this);    //形成输入规则
    ui->le_name->setValidator(validatorName);
    //密码正则
    QRegExp acNumRE_passwd("[0-9]{10}");//密码0~9 10位
    ui->le_passwd->setValidator(new QRegExpValidator(acNumRE_passwd, this));
    //注册窗口
    dialogRegist = new DialogRegist(this);
    dialogRegist->setWindowModality(Qt::ApplicationModal);
    connect(dialogRegist,SIGNAL(sigWrite(int,User)),this,SLOT(onSigWrite(int,User)));
    //用户窗口
    widgetUser = new WidgetUser(this);
    connect(widgetUser,SIGNAL(sigWrite(int,User)),this,SLOT(onSigWrite(int,User)));//收子
    connect(this,SIGNAL(sigToWidgetUser(int,User)),//发子
            widgetUser,SLOT(onSigToWidgetUser(int,User)));


    //创建套接字
    ip = "127.0.0.1";
    port = 8888;//服务器套接字端口号
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(onError(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    socket->connectToHost(QHostAddress(ip),port);



}



MainWindow::~MainWindow()
{
    delete ui;    
    delete widgetUser;//用户
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //客户端彻底退出之前发一个退出包给服务器
    User user;
    int type = TYPE_DISCONN;
    user.setType(TYPE_DISCONN);
    onSigWrite(type,user);
}



void MainWindow::onError(QAbstractSocket::SocketError socketError)
{
    QMessageBox::critical(this,"连接","连接服务器失败，请检查您的网络");
}

void MainWindow::on_btn_send_clicked()
{
//    Pack pack;
//    pack.type = TYPE_MSG;
//    strcpy(pack.message,ui->le_data->text().toLocal8Bit().data());
//    socket->write((char *)&pack,sizeof(Pack));
//    ui->textEdit->append("send:"+ui->le_data->text());
    //发弹幕现在放到直播界面去获取
}

/*
 * 函数名：onReadyRead
 * 参数：void
 * 返回值：void
 * 功能：监听服务器的数据包，解析包
 */
void MainWindow::onReadyRead()
{
    Pack pack;
    socket->read((char *)&pack,sizeof(Pack));    
    int type = pack.type;
    QString name = QString::fromLocal8Bit(pack.name);
    QString passwd = QString::fromLocal8Bit(pack.passwd);
    QString question = QString::fromLocal8Bit(pack.question);
    QString answer = QString::fromLocal8Bit(pack.answer);
    QString message = QString::fromLocal8Bit(pack.message);
    QString info = QString::fromLocal8Bit(pack.info);
    unsigned short int port = pack.port;
    int points = 0;
    User user;

    switch (type) {
    case TYPE_REG_SUCCESS:
        dialogRegist->hide();
        QMessageBox::information(dialogRegist,"注册",info);
        ui->textEdit->append(info);
        break;
    case TYPE_REG_ERROR:
        QMessageBox::critical(dialogRegist,"注册",info);
        ui->textEdit->append(info);
        break;
    case TYPE_LOG_SUCCESS:
        //QMessageBox::information(this,"登录",pack.info);
        ui->textEdit->append(info);
        this->hide();//隐藏登录界面

        widgetUser->show();//打开用户界面
        //打开用户界面后，发一个包给服务器，索要当前所有的房间信息
        //这套系统设计巧妙的地方
        //减少服务器负担，服务器不用定时发送房间信息
        //也可以借助这段代码实现手动刷新房间信息
        type = TYPE_PORTS;
        user.setType(type);
        onSigWrite(type,user);//将发往服务器
        break;
    case TYPE_LOG_ERROR:
        QMessageBox::critical(this,"登录",info);
        ui->textEdit->append(info);
        break;
    case TYPE_PORT:
        ui->textEdit->append(info);;
        break;
    case TYPE_CREATEROOM:
        ui->textEdit->append(info);
        //接下来要把这个申请到的广播port告诉Widgetuser
        user.setType(type);
        user.setInfo(info);
        user.setPort(port);
        sigToWidgetUser(type,user);        
        break;
    case TYPE_PORTS://接收索要的房间信息
        ui->textEdit->append(info);
        user.setType(type);
        user.setName(name);
        user.setPort(port);
        sigToWidgetUser(type,user);//发给Widgetuser
        break;
    case TYPE_MSG:
        user.setType(type);
        user.setName(name);
        user.setMessage(message);
        sigToWidgetUser(type,user);
        break;
    case TYPE_ROOMMEMBER:
        user.setName(name);
        sigToWidgetUser(type,user);
    default:
        break;
    }
}


void MainWindow::on_btn_reg_clicked()
{    
    dialogRegist->show();
}


void MainWindow::on_btn_login_clicked()
{    
    int type = TYPE_LOG;
    QString name = ui->le_name->text();
    QString passwd = ui->le_passwd->text();
    User user(type,name,passwd);
    onSigWrite(type,user);
    widgetUser->setName(name);//把登陆时的用户名传给后面的菜单
}

/*
 * 函数名：onSigWrite
 * 参数：int type, User user
 * 返回值：void
 * 功能：监听子界面的信号，集中转发各界面的数据包
 */
void MainWindow::onSigWrite(int type, User user)
{
    Pack pack;
    switch (type){
    case TYPE_REG:        
        pack.type = type;
        strcpy(pack.name,user.getName().toLocal8Bit().data());
        strcpy(pack.passwd,user.getPasswd().toLocal8Bit().data());
        strcpy(pack.question,user.getQuestion().toLocal8Bit().data());
        strcpy(pack.answer,user.getPasswd().toLocal8Bit().data());
        socket->write((char *)&pack,sizeof(Pack));
        ui->textEdit->append("注册信息已发往服务器");        
        break;
    case TYPE_LOG:
        pack.type = type;
        strcpy(pack.name,user.getName().toLocal8Bit().data());
        strcpy(pack.passwd,user.getPasswd().toLocal8Bit().data());
        socket->write((char *)&pack,sizeof(Pack));
        ui->textEdit->append("登录信息已发往服务器");
        break;
    case TYPE_MSG:
        pack.type = type;
        strcpy(pack.name,user.getName().toLocal8Bit().data());
        strcpy(pack.message,user.getMessage().toLocal8Bit().data());
        pack.port = user.getPort();
        socket->write((char *)&pack,sizeof(Pack));
        ui->textEdit->append("弹幕消息已发往服务器");
        break;
    case TYPE_PORT:
        pack.type = type;
        strcpy(pack.name,user.getName().toLocal8Bit().data());
        pack.port = user.getPort();
        socket->write((char *)&pack,sizeof(Pack));
        ui->textEdit->append("广播PORT已经发往服务器");
        break;
    case TYPE_CREATEROOM:
        pack.type = type;
        strcpy(pack.name,user.getName().toLocal8Bit().data());
        strcpy(pack.info,user.getInfo().toLocal8Bit().data());
        socket->write((char *)&pack,sizeof(Pack));
        ui->textEdit->append("申请广播port口消息已发往服务器");
        break;
    case TYPE_PORTS:
        pack.type = type;
        socket->write((char *)&pack,sizeof(Pack));
        ui->textEdit->append("索要房间信息已发往服务器");
        break;
    case TYPE_DISCONN:
        pack.type = type;
        socket->write((char *)&pack,sizeof(Pack));
        ui->textEdit->append("断开套接字请求已发往服务器");
        break;
    case TYPE_LOGOFF:
        pack.type = type;
        strcpy(pack.name,user.getName().toLocal8Bit().data());
        socket->write((char *)&pack,sizeof(Pack));
        ui->textEdit->append("下线请求已发往服务器");        
        break;
    case TYPE_INROOM:
        pack.type = type;
        strcpy(pack.name,user.getName().toLocal8Bit().data());
        pack.port = user.getPort();
        socket->write((char *)&pack,sizeof(Pack));
        ui->textEdit->append("加入房间请求已发往服务器");
        break;
    case TYPE_OUTROOM:
        pack.type = type;
        strcpy(pack.name,user.getName().toLocal8Bit().data());
        pack.port = user.getPort();
        socket->write((char *)&pack,sizeof(Pack));
        ui->textEdit->append("退出房间请求已发往服务器");
        break;

    default:
        break;
    }
}
//显示客户端日志
void MainWindow::on_btn_log_clicked()
{

}
//找回密码
void MainWindow::on_btn_backPasswd_clicked()
{

}
