#include "warehousetable.h"
#include <QPushButton>

WarehouseTable::WarehouseTable(QWidget *parent, JsonDownloader *jsonLoader)
    : QWidget(parent)
    , mainLayout(new QVBoxLayout)
    , tableWidget(new QTableWidget)
    , loader(jsonLoader)
    , jsonSender(new JsonSender)
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
    onNewWarehouseInfo();
}

void WarehouseTable::onNewWarehouseInfo()
{
    QJsonArray tableContent = loader->getWarehouseInfo();
    tableWidget->setRowCount(tableContent.size());
    for(int i = 0; i < tableContent.size(); i++){
        QJsonObject json = tableContent[i].toObject();

        int supply_id = json["id"].toInt();
        int ingredient_id = json["ingredient_id"].toInt();
        QJsonObject ingredientJson = loader->getIngredientById(ingredient_id);
        QString expiry_time = ingredientJson["expiry"].toString();
        QString ingredient_title = ingredientJson["title"].toString();

        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(supply_id)));
        tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(ingredient_id)));
        tableWidget->setItem(i, 2, new QTableWidgetItem(ingredient_title));
        tableWidget->setItem(i, 3, new QTableWidgetItem(json["mass"].toString()));
        tableWidget->setItem(i, 4, new QTableWidgetItem(expiry_time));
        tableWidget->setItem(i, 5, new QTableWidgetItem(json["date"].toString()));

        QPushButton *deleteThisButton = new QPushButton("Удалить");
        connect(deleteThisButton, &QPushButton::clicked, [supply_id,this](){
            jsonSender->deleteSupply(supply_id);
        });
        tableWidget->setCellWidget(i,6,deleteThisButton);
    }
}

void WarehouseTable::setupContents()
{
    mainLayout->addWidget(tableWidget);
    mainLayout->setContentsMargins(0,0,0,0);
    tableWidget->setColumnCount(7);
    tableWidget->setFont(QFont("Sans serif",12));
    for(int i = 0; i < 6; i++){
        tableWidget->setColumnWidth(i, 150);
    }
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("ID"));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("ID ингредиента"));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Название"));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Количество"));
    tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Срок годности"));
    tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Дата поставки"));
    tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem());
    //tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Конечный срок"));
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setFocusPolicy(Qt::NoFocus);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setLayout(mainLayout);
}
