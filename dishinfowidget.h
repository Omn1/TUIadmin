#ifndef DISHINFOWIDGET_H
#define DISHINFOWIDGET_H

#include <QWidget>

namespace Ui {
class DishInfoWidget;
}

class DishInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DishInfoWidget(QWidget *parent = nullptr);
    ~DishInfoWidget();

    void setTitle(QString title);
    void setPrice(QString price);
    void setMass(QString mass);
    void setImage(QPixmap pixmap);
private:
    Ui::DishInfoWidget *ui;
};

#endif // DISHINFOWIDGET_H
