#ifndef WIDGETLOOK_H
#define WIDGETLOOK_H

#include <QWidget>

namespace Ui {
class WidgetLook;
}

class WidgetLook : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetLook(QWidget *parent = 0);
    ~WidgetLook();

private:
    Ui::WidgetLook *ui;
};

#endif // WIDGETLOOK_H
