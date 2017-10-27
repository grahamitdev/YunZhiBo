#ifndef WIDGETUSER_H
#define WIDGETUSER_H

#include <QWidget>
#include "DialogLive.h"
#include "WidgetLook.h"
#include "User.h"
#include <QStandardItemModel>
#include <QModelIndex>
#include <QUdpSocket>
#include <QTimer>
namespace Ui {
class WidgetUser;
}

class WidgetUser : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetUser(QWidget *parentWidget, QWidget *parent = 0);
    ~WidgetUser();
    void setName(QString name);
    void closeEvent(QCloseEvent *event);
    void flushRoom();
    void *getParentWidget();


private slots:
    void on_btn_live_clicked();
    QWidget *getPerantWidget();
    void onSigWrite(int type,User user);//收子
    void onSigToWidgetUser(int type,User user);//收父
    void onReadyReadServer();//监听服务器的大广播



signals:
    void sigWrite(int type,User user);//发父
    void sigToWidgetLook(int type,User user);//发WidgetLook

private:
    Ui::WidgetUser *ui;
    QString QName;
    unsigned short int port;
    QWidget *parentWidget;
    DialogLive *dialogLive;//主播直播界面
    WidgetLook *widgetLook;//观众观看界面
    //接收服务器的大广播放在这个用户界面，不放在登录主界面
    unsigned short int portServer;
    QUdpSocket *receiverServer;//接收服务器大广播



private:
    QStandardItemModel *roomItemModel;
private slots:
    void itemClicked(QModelIndex index);    
    void on_btn_flush_clicked();
    void timerSingleClick();//定时器事件，处理单机任务
public:
    QTimer *pQTimerDoubleClicked;//判断单机双击的定时器
    int isDoubleClicked;//区分是单机还是双击


};

#endif // WIDGETUSER_H
