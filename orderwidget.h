#ifndef ORDERWIDGET_H
#define ORDERWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QToolButton>
#include <QJsonObject>
#include <QJsonArray>
#include "orderinfowidget.h"
#include "jsondownloader.h"

class OrderWidget : public QTreeWidgetItem
{
public:
    explicit OrderWidget(QTreeWidgetItem *parent);
    explicit OrderWidget(QTreeWidget *parent);
    ~OrderWidget();

    void setupContents();
    void loadFromJSON(const QJsonObject &json, JsonDownloader &loader);
private:
    QWidget *contents;
    QVBoxLayout *mainLayout;
    QToolButton *toggleButton;
    OrderInfoWidget *orderInfo;
};

#endif // ORDERWIDGET_H
