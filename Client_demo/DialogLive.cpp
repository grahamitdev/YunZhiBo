#include "DialogLive.h"
#include "ui_DialogLive.h"
#include "Packet.h"
#include <QMessageBox>
DialogLive::DialogLive(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLive)
{
    ui->setupUi(this);
    QCameraInfo info = QCameraInfo::defaultCamera();
    camera = new QCamera(info,this);
    camera->setViewfinder(ui->camera);
    portServer = 10000;
    port = 10001;
    sender = new QUdpSocket(this);

    receiverServer = new QUdpSocket(this);
    receiverServer->bind(portServer,QUdpSocket::ReuseAddressHint);
    connect(receiverServer,SIGNAL(readyRead()),this,SLOT(onReadyReadServer()));

    receiver = new QUdpSocket(this);
    receiver->bind(port,QUdpSocket::ReuseAddressHint);
    connect(receiver,SIGNAL(readyRead()),this,SLOT(onReadyRead()));    
}

DialogLive::~DialogLive()
{
    delete ui;
}

void DialogLive::closeEvent(QCloseEvent *event)
{
    this->parentWidget()->show();
}
//直播广播
void DialogLive::on_btn_start_clicked()
{
    //临时
    camera->start();
    char str[20] = "快来看直播";    
    sender->writeDatagram(str,1024,QHostAddress::Broadcast,port);
    int type = TYPE_PORT;
    User user(type,"小红",port);
    sigWrite(type,user);
}

void DialogLive::on_btn_stop_clicked()
{
    camera->stop();
}

void DialogLive::on_btn_close_clicked()
{
    camera->stop();
}

void DialogLive::onReadyRead()
{
    char str[20] = "";
    while(receiver->hasPendingDatagrams())
    {
        receiver->readDatagram(str,20);
        QString msg(str);
        ui->lb_public->setText(msg);
    }
}

void DialogLive::onReadyReadServer()
{
    Pack pack;
    while(receiverServer->hasPendingDatagrams())
    {
        receiverServer->readDatagram((char *)&pack,sizeof(Pack));
        QString name = QString::fromLocal8Bit(pack.name);
        QMessageBox::information(this,"直播消息",name+"开始");
    }
}
