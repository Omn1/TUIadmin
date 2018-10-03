#ifndef DISHADDERWIDGET_H
#define DISHADDERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "jsondownloader.h"
#include "jsonsender.h"
#include "ingredientselector.h"

namespace Ui {
class DishAdderWidget;
}

class DishAdderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DishAdderWidget(QWidget *parent = nullptr, JsonDownloader *jsonLoader = nullptr);
    ~DishAdderWidget();

    QJsonObject getInfo();
public slots:
    void onAddButtonClicked();
    void onSendButtonClicked();
private:
    Ui::DishAdderWidget *ui;
    JsonDownloader *loader;
    JsonSender *jsonSender;
    QPushButton *addButton;
};

#endif // DISHADDERWIDGET_H
