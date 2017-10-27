#include "DialogRegist.h"
#include "ui_DialogRegist.h"
#include <QMessageBox>
#include "Packet.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QMovie>
DialogRegist::DialogRegist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRegist)
{
    ui->setupUi(this);
    //gif
    QMovie *movie = new QMovie(":/new/prefix1/sun.gif");
    ui->label_6->setMovie(movie);
    movie->start();



    //用户名正则
    QRegExp regExpName("[0-9A-Za-z]{10}");//输入限定，即正则表达式，括号内的字符串是允许输入的内容
    QRegExpValidator* validatorName = new QRegExpValidator(regExpName,this);    //形成输入规则
    ui->le_name->setValidator(validatorName);
    //密码正则
    QRegExp acNumRE_passwd("[0-9]{10}");//密码0~9 10位
    ui->le_passwd->setValidator(new QRegExpValidator(acNumRE_passwd, this));
    ui->le_passwdConfirm->setValidator(new QRegExpValidator(acNumRE_passwd, this));
}

DialogRegist::~DialogRegist()
{
    delete ui;
}

void DialogRegist::on_btn_ok_clicked()
{
    int type = TYPE_REG;
    QString name = ui->le_name->text();
    QString passwd = ui->le_passwd->text();
    QString passwd_confirm = ui->le_passwdConfirm->text();
    QString question = "你最喜欢的水果？";
    QString answer = ui->le_answer->text();
    if(passwd != passwd_confirm)
    {
        QMessageBox::critical(this,"注册失败","两次密码不一致");
        return;
    }
    User user(type,name,passwd,question,answer);
    emit sigWrite(type,user);
}

void DialogRegist::on_btn_no_clicked()
{
    this->hide();
}
