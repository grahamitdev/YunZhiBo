#ifndef WIDGETUSER_H
#define WIDGETUSER_H

#include <QWidget>
#include "DialogLive.h"
#include "WidgetLook.h"
#include "User.h"
#include <QStandardItemModel>
#include <QModelIndex>
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

private slots:
    void on_btn_live_clicked();
    QWidget *getPerantWidget();
    void onSigWrite(int type,User user);//收子



signals:
    void sigWrite(int type,User user);//发父

private:
    Ui::WidgetUser *ui;
    QString name;
    QWidget *parentWidget;
    DialogLive *dialogLive;
    WidgetLook *widgetLook;

private:
    QStandardItemModel *roomItemModel;
private slots:
    void itemClicked(QModelIndex index);
};

#endif // WIDGETUSER_H
