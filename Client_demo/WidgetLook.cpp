#include "WidgetLook.h"
#include "ui_WidgetLook.h"
#include "ReceiveAudioThread.h"


WidgetLook::WidgetLook(QWidget *parentWidget,QString name, QString num, QString clientName,QWidget *parent) :
    QWidget(parent),parentWidget(parentWidget),name(name),num(num),clientName(clientName),
    ui(new Ui::WidgetLook)
{
    ui->setupUi(this);
    port = num.toUShort();
    ui->lb_room_info->setText("房间名:"+num+"\t主播名:"+name);

    //连接WidgetUser界面的槽
    connect(this,SIGNAL(sigWrite(int,User)),parentWidget,SLOT(onSigWrite(int,User)));
    socketRead = new QUdpSocket(this);    
    socketRead->bind(QHostAddress::AnyIPv4,port,QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress);
    connect(socketRead,SIGNAL(readyRead()),this,SLOT(onReadyReadSocket()));

    //视频
    imageBytes = new QByteArray(2000000,0);
    dataStream = new QDataStream(imageBytes,QIODevice::ReadWrite);
    //音频

    audioFormat.setSampleRate(8000);
    audioFormat.setChannelCount(1);
    audioFormat.setSampleSize(16);
    audioFormat.setCodec("audio/pcm");
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    audioFormat.setSampleType(QAudioFormat::SignedInt);
    audioOutput = new QAudioOutput(audioFormat);
    audioDevice = audioOutput->start();


}



WidgetLook::~WidgetLook()
{
    delete ui;
}

void WidgetLook::setName(QString name)
{
    this->name = name;
}

void WidgetLook::setNum(QString num)
{
    this->num = num;
}

QString WidgetLook::getName()
{
    return name;
}

QString WidgetLook::getNum()
{
    return num;
}

void WidgetLook::closeEvent(QCloseEvent *event)
{
    User user;
    int type = TYPE_OUTROOM;
    user.setType(type);
    user.setName(clientName);//观众名字
    user.setPort(port);//房间port
    emit sigWrite(type,user);//把退出房间请求发给主界面，由主界面发往服务器
    parentWidget->show();
}

void WidgetLook::updateMessage(User user)
{
    QString name = user.getName();
    QString message = user.getMessage();
    ui->te_msg->append(name+":"+message);
    //弹幕
    myBarrage = new MyBarrage(message,ui->lb_look->rect(),ui->lb_look);
}



void WidgetLook::onReadyReadSocket()
{
    while(socketRead->hasPendingDatagrams())
    {
        UPacket pack;
        memset(&pack, 0, sizeof(UPacket));
        socketRead->readDatagram((char*)&pack,sizeof(UPacket));
        switch (pack.type) {
        case TYPE_VIDEO:
            if(true == pack.isLastPack)
            {//将接收到的UDP视频包转换成图像播放在直播窗口
                dataStream->writeRawData(pack.data,pack.packTaken);
                image = QImage((uchar*)imageBytes->data(),
                                      pack.width,
                                      pack.height,
                                      pack.bytesPerline,
                                      QImage::Format_RGB32);
                map = QPixmap::fromImage(image);
                map = map.scaled(ui->lb_look->size());
                ui->lb_look ->setPixmap(map);
                dataStream->device()->seek(0);
            }
            else
            {
                dataStream->writeRawData(pack.data,pack.packTaken);
            }
            break;
        case TYPE_AUDIO:
            audioDevice->write(pack.data,pack.datalen);
            break;
        default:
            break;
        }
    }
}

void WidgetLook::onSigToWidgetLook(int type, User user)
{
    switch (type) {
    case TYPE_MSG:
        updateMessage(user);
        break;
    case TYPE_ROOMMEMBER:
        ui->te_mem->append(user.getName());
    default:
        break;
    }
}



void WidgetLook::on_btn_send_clicked()
{
    int type = TYPE_MSG;
    QString message = ui->le_msg->text();
    User user;
    user.setType(type);
    user.setName(clientName);
    user.setMessage(message);
    user.setPort(port);
    emit sigWrite(type,user);
}
