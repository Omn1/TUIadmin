#ifndef ORDERINFOWIDGET_H
#define ORDERINFOWIDGET_H

#include <QWidget>

namespace Ui {
class OrderInfoWidget;
}

class OrderInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OrderInfoWidget(QWidget *parent = nullptr);
    ~OrderInfoWidget();

    void setID(QString id);
    void setAddress(QString address);
    void setPhone(QString phone);
    void setName(QString name);
private:
    Ui::OrderInfoWidget *ui;
};

#endif // ORDERINFOWIDGET_H
