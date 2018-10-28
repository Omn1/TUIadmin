#include "ordertable.h"
#include "sessioninfo.h"
#include <QDebug>

OrderTable::OrderTable(QWidget *parent, JsonDownloader *jsonloader)
    : QWidget(parent)
    , mainLayout(new QVBoxLayout)
    , orderTypeBox(new QComboBox)
    , treeWidget(new QTreeWidget)
    , loader(jsonloader)
    , jsonSender(new JsonSender)
    , displayed_order_id(-1)
    , displayed_order_status(-1)
{

    orderTypeBox->addItem("Все активные заказы",-1);
    orderTypeBox->addItem("Ожидают подтверждения",0);
    orderTypeBox->addItem("Готовятся",1);
    orderTypeBox->addItem("Ожидают доставки",2);
    orderTypeBox->addItem("Доставляются",3);

    connect(orderTypeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &OrderTable::onNewOrders);

    mainLayout->addWidget(orderTypeBox);

    setupTreeWidget();

    setLayout(mainLayout);

    if (loader == nullptr) {
        loader = new JsonDownloader;
        connect(loader, &JsonDownloader::updateReady, this, &OrderTable::onNewOrders);
        loader->start();
    }
    else{
        connect(loader, &JsonDownloader::updateReady, this, &OrderTable::onNewOrders);
    }

    onNewOrders();
}

void OrderTable::reloadOrders()
{
    int scrollBarPosition = treeWidget->verticalScrollBar()->value();
    delete treeWidget;
    treeWidget = new QTreeWidget;
    setupTreeWidget();
    displayed_order_status = orderTypeBox->itemData(orderTypeBox->currentIndex()).toInt();
    for (int i = 0; i < orders.size(); i++)
    {
        QJsonObject json = orders[i].toObject();
        if((displayed_order_id == -1 || json["id"].toInt() == displayed_order_id)
            && json["status"].toInt() != 4
            && (displayed_order_status == -1 || displayed_order_status == json["status"].toInt()))
        {
            OrderWidget *order = new OrderWidget(treeWidget);
            order->loadFromJSON(json, *loader);
            order->setFont(1,QFont("Sans serif",16));
            QString statusText;
            int statusInt = json["status"].toInt();
            int orderId = json["id"].toInt();
            if (statusInt == 0) {
                statusText = "Ожидает подтверждения";
                order->setBackgroundColor(1,QColor(255,0,0,127));
                QPushButton *confirmButton = new QPushButton("Подтвердить");
                confirmButton->setMaximumWidth(200);
                confirmButton->setFont(QFont("Sans serif",16));
                connect(confirmButton, &QPushButton::clicked, [this,orderId]{
                    jsonSender->confirmOrder(orderId);
                });
                treeWidget->setItemWidget(order,2,confirmButton);
                confirmButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::OPERATOR_PERMISSION));
                QPushButton *declineButton = new QPushButton("Отклонить");
                declineButton->setMaximumWidth(200);
                declineButton->setFont(QFont("Sans serif",16));
                connect(declineButton, &QPushButton::clicked, [this,orderId]{
                    jsonSender->declineOrder(orderId);
                });
                treeWidget->setItemWidget(order,3,declineButton);
                declineButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::OPERATOR_PERMISSION));
            }
            else if (statusInt == 1) {
                statusText = "Готовится";
                order->setBackgroundColor(1,QColor(255,255,0,127));
                QPushButton *cookButton = new QPushButton("Приготовлено");
                cookButton->setMaximumWidth(200);
                cookButton->setFont(QFont("Sans serif",16));
                connect(cookButton, &QPushButton::clicked, [this,orderId]{
                    jsonSender->cookOrder(orderId);
                });
                treeWidget->setItemWidget(order,2,cookButton);
                cookButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::COOK_PERMISSION));
            }
            else if (statusInt == 2) {
                statusText = "Ожидает доставки";
                order->setBackgroundColor(1,QColor(127,255,0,127));
            }
            else if (statusInt == 3) {
                statusText = "Доставляется";
                order->setBackgroundColor(1,QColor(0,255,0,127));
            }
            order->setText(1,"Статус заказа:\n"+statusText);

        }
    }
    treeWidget->verticalScrollBar()->setMaximum(scrollBarPosition);
    treeWidget->verticalScrollBar()->setValue(scrollBarPosition);
}

void OrderTable::setDisplayedOrder(int displayedOrder)
{
    displayed_order_id = displayedOrder;
}

void OrderTable::onNewOrders()
{
    qDebug() << "New Orders!!\n";
    orders = loader->getOrders();

    reloadOrders();
}

void OrderTable::setupTreeWidget()
{
    treeWidget->header()->hide();
    treeWidget->setAnimated(true);
    treeWidget->setColumnCount(4);
    treeWidget->setColumnWidth(1,320);
    treeWidget->setColumnWidth(2,200);
    treeWidget->setColumnWidth(3,200);
    treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    treeWidget->setFocusPolicy(Qt::NoFocus);
    treeWidget->setRootIsDecorated(false);
    treeWidget->setIndentation(0);
    treeWidget->setExpandsOnDoubleClick(false);
    treeWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mainLayout->addWidget(treeWidget);
}
