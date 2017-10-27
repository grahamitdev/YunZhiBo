#ifndef MYBARRAGE_H
#define MYBARRAGE_H

#include <QLabel>
#include <QPropertyAnimation>
#include <QPalette>
#include <QFontMetrics>

class MyBarrage : public QLabel
{
    Q_OBJECT
public:
    explicit MyBarrage(QString str, QRect parentRect, QWidget *parent = nullptr);


signals:


public slots:
    void onAnimaFinished();

private:
    QPropertyAnimation *anima;
};

#endif // MYBARRAGE_H
