#include "WidgetLive.h"
#include "ui_WidgetLive.h"
#include <QDebug>
WidgetLive::WidgetLive(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetLive)
{
    ui->setupUi(this);
}

WidgetLive::~WidgetLive()
{
    delete ui;
}

void WidgetLive::closeEvent(QCloseEvent *event)
{
    qDebug()<<"拦截";

}
