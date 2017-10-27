#ifndef WIDGETLOOK_H
#define WIDGETLOOK_H

#include <QWidget>
#include "User.h"
#include "Packet.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QUdpSocket>
#include <QCameraImageCapture>
#include <QDebug>
#include "DialogLive.h"
#include <QThread>
#include <QIODevice>
#include <QAudioInput>
#include <QAudioOutput>
#include "MyBarrage.h"

namespace Ui {
class WidgetLook;
}


class WidgetLook : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetLook(QWidget *parentWidget,QString name,QString num,QString clientName,QWidget *parent = 0);
    ~WidgetLook();
    void setName(QString name);
    void setNum(QString num);
    QString getName();
    QString getNum();
    void closeEvent(QCloseEvent *event);

    void updateMessage(User user);



private:
    Ui::WidgetLook *ui;
    QWidget *parentWidget;
    QString name;//主播名字
    QString num;//房间号
    QString clientName;//观众名字
    unsigned short int port;//房间号
    QUdpSocket *socketRead;    
    QThread *thread;

    QAudioDeviceInfo deviceInfo;
    QIODevice *audioDevice;
    QAudioFormat audioFormat;
    QAudioOutput* audioOutput;
    MyBarrage *myBarrage;

private slots:
    void onReadyReadSocket();//收广播
    void onSigToWidgetLook(int type,User user);//收WidgetUser
    void on_btn_send_clicked();


signals:
    void sigWrite(int type,User user);//发WidgetUser


public:
    QByteArray *imageBytes;
    QDataStream *dataStream;
    QImage image;
    QPixmap map;
};

#endif // WIDGETLOOK_H
