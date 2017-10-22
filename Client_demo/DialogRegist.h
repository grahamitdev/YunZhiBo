#ifndef DIALOGREGIST_H
#define DIALOGREGIST_H

#include <QDialog>
#include "User.h"
namespace Ui {
class DialogRegist;
}

class DialogRegist : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRegist(QWidget *parent = 0);
    ~DialogRegist();

private slots:
    void on_btn_ok_clicked();
    void on_btn_no_clicked();
signals:
    void sigWrite(int type,User user);
private:
    Ui::DialogRegist *ui;
};

#endif // DIALOGREGIST_H
