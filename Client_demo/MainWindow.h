#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "DialogRegist.h"
#include "DialogLive.h"
#include "WidgetUser.h"
#include "WidgetLive.h"
#include "User.h"
namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);    
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
private slots:    
    void onError(QAbstractSocket::SocketError socketError);
    void on_btn_send_clicked();
    void onReadyRead();
    void on_btn_log_clicked();
    void on_btn_reg_clicked();
    void on_btn_login_clicked();
    void onSigWrite(int type,User user);
    void on_btn_backPasswd_clicked();

signals:
    void sigToWidgetUser(int type,User user);//向WidgetUser界面发信号

private:
    Ui::MainWindow *ui;    
    DialogRegist *dialogRegist;//注册
    WidgetUser *widgetUser;//用户
//    DialogLive *dialogLive;
//    WidgetLive *widgetLive;


    QString ip;
    unsigned short int port;
    QTcpSocket *socket;

};

#endif // MAINWINDOW_H
