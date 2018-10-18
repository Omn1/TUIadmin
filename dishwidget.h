#ifndef DISHWIDGET_H
#define DISHWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QToolButton>
#include "dishinfowidget.h"
#include "jsondownloader.h"

class DishWidget : public QTreeWidgetItem
{
public:
    explicit DishWidget(QTreeWidgetItem *parent = nullptr);
    explicit DishWidget(QTreeWidget *parent = nullptr);
    ~DishWidget();

    void setupContents();
    void loadFromJSON(const QJsonObject &json, JsonDownloader &loader);
private:
    QWidget *contents;
    QVBoxLayout *mainLayout;
    QToolButton *toggleButton;
    DishInfoWidget *dishInfo;
};

#endif // DISHWIDGET_H

