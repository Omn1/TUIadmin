#include "warehousetable.h"

WarehouseTable::WarehouseTable(QWidget *parent, JsonDownloader *jsonLoader)
    : QWidget(parent)
    , mainLayout(new QVBoxLayout)
    , tableWidget(new QTableWidget)
    , loader(jsonLoader)
{
    if(!loader){
        loader = new JsonDownloader;
        connect(loader, &JsonDownloader::updateReady, this, &WarehouseTable::onNewWarehouseInfo);
        loader->start();
    }
    else{
        connect(loader, &JsonDownloader::updateReady, this, &WarehouseTable::onNewWarehouseInfo);
    }

    setupContents();
}

void WarehouseTable::onNewWarehouseInfo()
{
    QJsonArray tableContent = loader->getWarehouseInfo();
    tableWidget->setRowCount(tableContent.size());
    for(int i = 0; i < tableContent.size(); i++){
        QJsonObject json = tableContent[i].toObject();

        int ingredient_id = json["ingredient_id"].toInt();
        QJsonObject ingredientJson = loader->getIngredientById(ingredient_id);
        QString expiry_time = ingredientJson["expiry"].toString();
        QString ingredient_title = ingredientJson["title"].toString();

        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(json["id"].toInt())));
        tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(ingredient_id)));
        tableWidget->setItem(i, 2, new QTableWidgetItem(ingredient_title));
        tableWidget->setItem(i, 3, new QTableWidgetItem(json["mass"].toString()));
        tableWidget->setItem(i, 4, new QTableWidgetItem(expiry_time));
        tableWidget->setItem(i, 5, new QTableWidgetItem(json["date"].toString()));
    }
}

void WarehouseTable::setupContents()
{
    mainLayout->addWidget(tableWidget);
    tableWidget->setColumnCount(6);
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("ID"));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("ID ингредиента"));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Название"));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Количество"));
    tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Срок годности"));
    tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Дата поставки"));
    //tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Конечный срок"));
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setFocusPolicy(Qt::NoFocus);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setLayout(mainLayout);
}
