#include "ordertable.h"
#include <QDebug>

OrderTable::OrderTable(QWidget *parent, JsonDownloader *jsonloader)
    : QWidget(parent)
    , mainLayout(new QVBoxLayout)
    , treeWidget(new QTreeWidget)
    , scrollArea(new QScrollArea)
    , loader(jsonloader)
    , displayed_order_id(-1)
{
    //treeWidget->setStyleSheet("OrderInfoWidget{border:1px solid red}");
    treeWidget->header()->hide();
    treeWidget->setAnimated(true);
    treeWidget->setColumnCount(2);
    treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    treeWidget->setFocusPolicy(Qt::NoFocus);
    treeWidget->setRootIsDecorated(false);
    treeWidget->setIndentation(0);
    treeWidget->setExpandsOnDoubleClick(false);
    //treeWidget->setStyleSheet("QTreeWidget::item { border-bottom: 1px solid black;}");

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
}

void OrderTable::reloadOrders()
{
    treeWidget->clear();

    for (int i = 0; i < orders.size(); i++)
    {
        QJsonObject json = orders[i].toObject();
        if(displayed_order_id == -1 || json["id"].toInt() == displayed_order_id)
        {
            OrderWidget *order = new OrderWidget(treeWidget);
            order->loadFromJSON(json, *loader);
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
