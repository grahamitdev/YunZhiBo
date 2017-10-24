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

    sender = new QUdpSocket(this);


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
    //this->parentWidget()->show();
}
//直播广播
void DialogLive::on_btn_start_clicked()
{
//    //临时
//    camera->start();
//    char str[20] = "快来看直播";
//    sender->writeDatagram(str,1024,QHostAddress::Broadcast,port);
//    int type = TYPE_PORT;
//    User user(type,"小红",port);//把开直播的广播Port封到包里
//    sigWrite(type,user);//通过Tcp发给服务器
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
//    char str[20] = "";
//    while(receiver->hasPendingDatagrams())
//    {
//        receiver->readDatagram(str,20);
//        QString msg(str);
//        ui->lb_public->setText(msg);
//    }
}

//弹幕消息通过Tcp发给服务器，服务器通过广播转发给相同房间(监听相同PORT)的其他用户
void DialogLive::on_btn_send_clicked()
{
    int type = TYPE_MSG;
    QString name = "小红";
    QString message = ui->le_msg->text();
    User user(type,name,message);
    sigWrite(type,user); //发给上一级窗口
}
