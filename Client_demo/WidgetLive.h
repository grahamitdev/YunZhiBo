#ifndef WIDGETLIVE_H
#define WIDGETLIVE_H

#include <QWidget>

namespace Ui {
class WidgetLive;
}

class WidgetLive : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetLive(QWidget *parent = 0);
    ~WidgetLive();
    void closeEvent(QCloseEvent *event);

private:
    Ui::WidgetLive *ui;
};

#endif // WIDGETLIVE_H
