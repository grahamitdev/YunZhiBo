#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "DialogRegist.h"
#include "DialogLive.h"
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
private slots:
    void onError(QAbstractSocket::SocketError socketError);
    void on_btn_send_clicked();
    void onReadyRead();
    void on_btn_log_clicked();
    void on_btn_reg_clicked();
    void on_btn_login_clicked();
    void onSigWrite(int type,User user);
    void on_btn_backPasswd_clicked();

private:
    Ui::MainWindow *ui;    
    DialogRegist *dialogRegist;
    DialogLive *dialogLive;
    WidgetLive *widgetLive;


    QString ip;
    unsigned short int port;
    QTcpSocket *socket;

};

#endif // MAINWINDOW_H
