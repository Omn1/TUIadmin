#include "ordertable.h"
#include <QDebug>

OrderTable::OrderTable(QWidget *parent, JsonDownloader *jsonloader)
    : QWidget(parent)
    , mainLayout(new QVBoxLayout)
    , orderTypeBox(new QComboBox)
    , treeWidget(new QTreeWidget)
    , loader(jsonloader)
    , displayed_order_id(-1)
    , displayed_order_status(-1)
{
    //treeWidget->setStyleSheet("OrderInfoWidget{border:1px solid red}");
    treeWidget->header()->hide();
    treeWidget->setAnimated(true);
    treeWidget->setColumnCount(3);
    treeWidget->setColumnWidth(1,320);
    treeWidget->setColumnWidth(2,200);
    treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    treeWidget->setFocusPolicy(Qt::NoFocus);
    treeWidget->setRootIsDecorated(false);
    treeWidget->setIndentation(0);
    treeWidget->setExpandsOnDoubleClick(false);
    treeWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    //treeWidget->setStyleSheet("QTreeWidget::item { border-bottom: 1px solid black;}");

    orderTypeBox->addItem("Все активные заказы",-1);
    orderTypeBox->addItem("Ожидают пожтверждения",0);
    orderTypeBox->addItem("Готовятся",1);
    orderTypeBox->addItem("Ожидают доставки",2);
    orderTypeBox->addItem("Доставляются",3);

    connect(orderTypeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &OrderTable::onNewOrders);

    mainLayout->addWidget(orderTypeBox);
    mainLayout->addWidget(treeWidget);
    /*//treeWidget->addTopLevelItem(treeWidgetItem);

    QVBoxLayout *vLayout = new QVBoxLayout;

    vLayout->addWidget(treeWidget);

    QWidget *scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(vLayout);

    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollAreaContent);

    scrollArea->show();

    //mainLayout->addWidget(scrollArea);
*/
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
    treeWidget->clear();
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
            if (statusInt == 0) {
                statusText = "Ожидает подтверждения";
                order->setBackgroundColor(1,QColor(255,0,0,127));
            }
            else if (statusInt == 1) {
                statusText = "Готовится";
                order->setBackgroundColor(1,QColor(255,255,0,127));
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
