#include "WidgetLook.h"
#include "ui_WidgetLook.h"

WidgetLook::WidgetLook(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetLook)
{
    ui->setupUi(this);
}

WidgetLook::~WidgetLook()
{
    delete ui;
}
