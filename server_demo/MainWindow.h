#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include "Packet.h"
#include "User.h"
#include "QUdpSocket"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createDatabaseTable();

private slots:
    void onPrintLog(QString log);//主线程监听对象发射的打印log信号和log信息
    void on_btn_listen_clicked();
    void on_btn_reboot_clicked();
    void onNewConnection();    
    void onSigWriteToClient(QTcpSocket*,Pack);
    void onSigSendPortByBroad(Pack pack);
    void onDisconnected();
    void onChangeNumSocket(int num);
    void onChangeNumUser(int num);
    void onChangeNumRoom(int num);
    void on_btn_send_public_clicked();

private:
    Ui::MainWindow *ui;
    QString ip;
    unsigned short int port;
    unsigned short int port_broadcast;
    QTcpServer *server;
    QUdpSocket *sender;//全局广播：发新主播name+port

    int num_user;//用于计算服务器压力
    int num_room;
    int num_socket;


};

#endif // MAINWINDOW_H
