#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QHostAddress>
#include <QMessageBox>
#include <string.h>
#include <QDebug>
#include <string.h>
#include "Packet.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    //注册窗口
    dialogRegist = new DialogRegist(this);
    dialogRegist->setWindowModality(Qt::ApplicationModal);
    connect(dialogRegist,SIGNAL(sigWrite(int,User)),this,SLOT(onSigWrite(int,User)));
    //用户窗口
    widgetUser = new WidgetUser(this);
    connect(widgetUser,SIGNAL(sigWrite(int,User)),this,SLOT(onSigWrite(int,User)));

//    //直播窗口，可能不在这里创建
//    dialogLive = new DialogLive(this);
//    dialogLive ->setWindowModality(Qt::ApplicationModal);
//    connect(dialogLive,SIGNAL(sigWrite(int,User)),this,SLOT(onSigWrite(int,User)));

    //测试
    widgetLive = new WidgetLive();
    //创建套接字
    ip = "127.0.0.1";
    port = 8888;
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(onError(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    socket->connectToHost(QHostAddress(ip),port);



}

MainWindow::~MainWindow()
{
    delete ui;
    delete widgetLive;//直播
    delete widgetUser;//用户
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
        this->hide();
        widgetUser->setName(name);
        widgetUser->show();//打开直播界面
        break;
    case TYPE_LOG_ERROR:
        QMessageBox::critical(this,"登录",info);
        ui->textEdit->append(info);
        break;
    case TYPE_PORT:
        ui->textEdit->append(info);;
        break;
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
}



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
        //pack.info = ty

    default:
        break;
    }
}

void MainWindow::on_btn_log_clicked()
{

}

void MainWindow::on_btn_backPasswd_clicked()
{
    this->hide();
    widgetLive->show();
}
