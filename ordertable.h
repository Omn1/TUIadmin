#ifndef ORDERTABLE_H
#define ORDERTABLE_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QTreeWidget>
#include <QHeaderView>
#include <QJsonObject>
#include <QJsonArray>
#include <QComboBox>
#include <QPushButton>
#include "orderwidget.h"
#include "dishwidget.h"
#include "jsondownloader.h"
#include "jsonsender.h"

class OrderTable : public QWidget
{
    Q_OBJECT
public:
    explicit OrderTable(QWidget *parent = nullptr, JsonDownloader *loader = nullptr);

    void reloadOrders();
    void setDisplayedOrder(int displayedOrder);
signals:

public slots:
    void onNewOrders();
private:
    void setupTreeWidget();
    QVBoxLayout *mainLayout;
    QComboBox *orderTypeBox;
    QTreeWidget *treeWidget;
    JsonDownloader *loader;
    JsonSender *jsonSender;
    QJsonArray orders;
    int displayed_order_id;
    int displayed_order_status;
};

#endif // ORDERTABLE_H
