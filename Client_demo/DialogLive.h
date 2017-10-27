#ifndef DIALOGLIVE_H
#define DIALOGLIVE_H

#include <QDialog>
#include <QCamera>
#include <QCameraInfo>
#include <QUdpSocket>
#include "User.h"
#include <QListView>
#include <QStringListModel>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <string.h>
#include <QString>
#include "VideoSurface.h"
#include <QIODevice>
#include <QAudioInput>

#define TYPE_VIDEO 1
#define TYPE_AUDIO 2

typedef struct
{
    int type;
    int width;
    int height;
    int bytesPerline;
    bool isLastPack;
    int packTaken;
    int datalen;
    char data[1024];
}UPacket;



namespace Ui {
class DialogLive;
}

class DialogLive : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLive(QString name,QString num,QWidget *parent = 0);
    ~DialogLive();    
    void closeEvent(QCloseEvent *event);
    void setName(QString name);
    void setNum(QString num);
    QString getName();
    QString getNum();

private slots:
    void on_btn_start_clicked();
    void on_btn_stop_clicked();
    void on_btn_close_clicked();
    void onReadyRead();
    void on_btn_send_clicked();

    void onVideoChanged(QVideoFrame);
    void on_audioIOdevice();

signals:
    void sigWrite(int type,User user);//向上一级界面WidgetUser发送数据对象

private:
    Ui::DialogLive *ui;
    QWidget *parentwidget;
    QString name;
    QString num;
    unsigned short int port;
    QUdpSocket *sender;    
    QUdpSocket *receiver;
    //广播：255,255,255,255
    //组播：224.0.0.0至239.255.255.255

    //视频
    QCamera *camera;// 系统摄像设备（摄像头）
    QCameraViewfinder *cameraViewFinder;// 摄像取景器部件
    QCameraImageCapture *cameraImageCapture;// 截图部件
    //声音
    QAudioDeviceInfo deviceInfo;
    QIODevice *audioDevice;
    QAudioFormat audioFormat;
    QAudioInput* audioInput;



};

#endif // DIALOGLIVE_H
