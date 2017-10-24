#ifndef DIALOGLIVE_H
#define DIALOGLIVE_H

#include <QDialog>
#include <QCamera>
#include <QCameraInfo>
#include <QUdpSocket>
#include "User.h"
namespace Ui {
class DialogLive;
}

class DialogLive : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLive(QWidget *parent = 0);
    ~DialogLive();
    void closeEvent(QCloseEvent *event);

private slots:
    void on_btn_start_clicked();
    void on_btn_stop_clicked();
    void on_btn_close_clicked();
    void onReadyRead();

    void on_btn_send_clicked();

signals:
    void sigWrite(int type,User user);//向上一级界面发送数据对象

private:
    Ui::DialogLive *ui;
    QCamera *camera;
    unsigned short int port;    
    QUdpSocket *sender;
    QUdpSocket *receiver;

};

#endif // DIALOGLIVE_H
