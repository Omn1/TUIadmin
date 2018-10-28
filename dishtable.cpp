#include "dishtable.h"
#include "sessioninfo.h"

DishTable::DishTable(QWidget *parent, JsonDownloader *jsonloader)
    : QWidget(parent)
    , mainLayout(new QVBoxLayout)
    , treeWidget(new QTreeWidget)
    , loader(jsonloader)
    , jsonSender(new JsonSender)
{
    treeWidget->header()->hide();
    treeWidget->setAnimated(true);
    treeWidget->setColumnCount(3);
    treeWidget->setColumnWidth(1,300);
    treeWidget->setColumnWidth(2,300);
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
    int scrollBarPosition = treeWidget->verticalScrollBar()->value();
    treeWidget->clear();

    for (int i = 0; i < dishes.size(); i++)
    {
        QJsonObject json = dishes[i].toObject();
        int dish_id = json["id"].toInt();
        DishWidget *dish = new DishWidget(treeWidget);
        dish->loadFromJSON(json, *loader);
        QPushButton *statsButton = new QPushButton("Смотреть статистику");
        statsButton->setFont(QFont("Sans serif",12));
        connect(statsButton, &QPushButton::clicked, [this,dish_id]{
            QChartView *cv = new QChartView(statEngine->getRecentDishOrdersChart(dish_id,7));
            cv->setRenderHint(QPainter::Antialiasing);
            cv->setMinimumSize(800,600);
            cv->show();
        });
        QPushButton *deleteButton = new QPushButton("Удалить из меню");
        deleteButton->setMaximumWidth(300);
        deleteButton->setFont(QFont("Sans serif",12));
        connect(deleteButton, &QPushButton::clicked, [this,dish_id]{
            jsonSender->deleteDish(dish_id);
        });
        treeWidget->setItemWidget(dish,1,statsButton);
        treeWidget->setItemWidget(dish,2,deleteButton);
        deleteButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::MENU_MGR_PERMISSION));
    }
    treeWidget->verticalScrollBar()->setMaximum(scrollBarPosition);
    treeWidget->verticalScrollBar()->setValue(scrollBarPosition);
}

void DishTable::onNewDishes()
{
    dishes = loader->getDishes();

    reloadDishes();
}
