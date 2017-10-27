#include "MyBarrage.h"
#include <QDebug>

MyBarrage::MyBarrage(QString str,QRect parentRect, QWidget *parent) : QLabel(parent)
{
    this->setText(str);
    QFont font = QFont("SimHei",15,100);//字体大小
    this->setFont(font);
    QPalette pa;
    pa.setColor(QPalette::Foreground,QColor(255,255,255));
    this->setPalette(pa);

    QFontMetrics metrics(font);
    int y = qrand()%(parentRect.height() - metrics.height());

    anima = new QPropertyAnimation(this, "pos");
    anima->setDuration(8000);
    anima->setEasingCurve(QEasingCurve::InQuad);
    anima->setStartValue(QPoint(parentRect.width(), y));
    anima->setEndValue(QPoint(-parentRect.width(), y));
    this->show();
    anima->start();
    connect(anima,SIGNAL(finished()),this,SLOT(onAnimaFinished()));
}

void MyBarrage::onAnimaFinished()
{
    this->deleteLater();
}
