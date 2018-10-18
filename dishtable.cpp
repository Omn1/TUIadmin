#include "dishtable.h"

DishTable::DishTable(QWidget *parent, JsonDownloader *jsonloader)
    : QWidget(parent)
    , mainLayout(new QVBoxLayout)
    , treeWidget(new QTreeWidget)
    , loader(jsonloader)
{
    treeWidget->header()->hide();
    treeWidget->setAnimated(true);
    treeWidget->setColumnCount(3);
    treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    treeWidget->setFocusPolicy(Qt::NoFocus);
    treeWidget->setRootIsDecorated(false);
    treeWidget->setIndentation(0);
    treeWidget->setExpandsOnDoubleClick(false);
    treeWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    mainLayout->addWidget(treeWidget);
    setLayout(mainLayout);

    if (loader == nullptr) {
        loader = new JsonDownloader;
        connect(loader, &JsonDownloader::updateReady, this, &DishTable::onNewDishes);
        loader->start();
    }
    else{
        connect(loader, &JsonDownloader::updateReady, this, &DishTable::onNewDishes);
    }
    statEngine = new StatisticsEngine(nullptr,loader);

    onNewDishes();
}

void DishTable::reloadDishes()
{
    treeWidget->clear();

    for (int i = 0; i < dishes.size(); i++)
    {
        QJsonObject json = dishes[i].toObject();
        int dish_id = json["id"].toInt();
        DishWidget *dish = new DishWidget(treeWidget);
        dish->loadFromJSON(json, *loader);
        QPushButton *statsButton = new QPushButton("Смотреть статистику");
        statsButton->setMinimumWidth(150);
        connect(statsButton, &QPushButton::clicked, [this,dish_id]{
            QChartView *cv = new QChartView(statEngine->getRecentDishOrdersChart(dish_id,7));
            cv->setRenderHint(QPainter::Antialiasing);
            cv->setMinimumSize(800,600);
            cv->show();
        });
        treeWidget->setItemWidget(dish,1,statsButton);
    }
}

void DishTable::onNewDishes()
{
    dishes = loader->getDishes();

    reloadDishes();
}
