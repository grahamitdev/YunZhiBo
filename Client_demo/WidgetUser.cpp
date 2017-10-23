#include "WidgetUser.h"
#include "ui_WidgetUser.h"
#include "Packet.h"
#include "User.h"

WidgetUser::WidgetUser(QWidget *parentWidget,QWidget *parent) :
    QWidget(parent),parentWidget(parentWidget),
    ui(new Ui::WidgetUser)
{
    ui->setupUi(this);
    dialogLive = new DialogLive(this);
    connect(dialogLive,SIGNAL(sigWrite(int,User)),this,SLOT(onSigWrite(int,User)));

    widgetLook = new WidgetLook();

    roomItemModel = new QStandardItemModel(ui->listView);
    QStringList strList;
    strList.append("a");
    strList.append("b");
    strList.append("c");
    strList.append("d");
    strList.append("e");
    strList.append("f");
    strList.append("g");
    int nCount = strList.size();
    int i = 0;
    for(i = 0; i < nCount; i++)
    {
        QString string = static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        if(i %2 == 1)
        {
            QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
            linearGrad.setColorAt(0, Qt::white);
            linearGrad.setColorAt(1, Qt::red);
            QBrush brush(linearGrad);
            item->setBackground(brush);
        }
        QPixmap pixmap(":/new/prefix1/logo_R.ico");//设定图片
        item->setIcon(pixmap);
        roomItemModel->appendRow(item);
    }
    ui->listView->setModel(roomItemModel);//显示model中的内容
    connect(ui->listView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(itemClicked(QModelIndex)));
}

WidgetUser::~WidgetUser()
{
    delete ui;
    delete widgetLook;
}

void WidgetUser::setName(QString name)
{
    this -> name = name;
}

void WidgetUser::closeEvent(QCloseEvent *event)
{
    parentWidget->show();
}

void WidgetUser::on_btn_live_clicked()
{
    dialogLive->show();
    int type = TYPE_CREATEROOM;//去向服务器要port口
    QString name = name;
    QString str = "create";
    User user(type,name,str);
    sigWrite(type,user);//在主播打开摄像头之前就去要port，先建一个房间用于聊天
}

QWidget *WidgetUser::getPerantWidget()
{
    return parentWidget;
}

void WidgetUser::onSigWrite(int type, User user)
{
    emit sigWrite(type,user);
}

void WidgetUser::itemClicked(QModelIndex index)
{
    dialogLive->show();
}
