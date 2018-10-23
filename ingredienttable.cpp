#include "ingredienttable.h"
#include <QHeaderView>
#include <QPushButton>
#include "ingredientwidget.h"

IngredientTable::IngredientTable(QWidget *parent, JsonDownloader *jsonLoader)
    : QWidget(parent)
    , mainLayout(new QVBoxLayout)
    , tableWidget(new QTableWidget)
    , loader(jsonLoader)
    , jsonSender(new JsonSender)
{
    if(!loader){
        loader = new JsonDownloader;
        connect(loader, &JsonDownloader::updateReady, this, &IngredientTable::onNewIngredients);
        loader->start();
    }
    else{
        connect(loader, &JsonDownloader::updateReady, this, &IngredientTable::onNewIngredients);
    }

    setupContents();
    onNewIngredients();
}

void IngredientTable::onNewIngredients()
{
    QJsonArray tableContent = loader->getIngredients();
    tableWidget->setRowCount(tableContent.size());
    for (int i = 0; i < tableWidget->rowCount(); i++) {
        QJsonObject json = tableContent[i].toObject();
        IngredientWidget *ingredient = new IngredientWidget;
        ingredient->loadFromJSON(json, *loader);
        tableWidget->setRowHeight(i,ingredient->height());
        tableWidget->setCellWidget(i,0,ingredient);

        tableWidget->setItem(i, 1, new QTableWidgetItem(json["expiry"].toString()));

        int ingredient_id = json["id"].toInt();

        QPushButton *warehouseButton = new QPushButton("Посмтреть на складе");
        connect(warehouseButton, &QPushButton::clicked, [this,ingredient_id]{
            emit checkWarehouse(ingredient_id);
        });
        tableWidget->setCellWidget(i,2,warehouseButton);

        QPushButton *deleteButton = new QPushButton("Удалить");
        connect(deleteButton, &QPushButton::clicked, [this,ingredient_id]{
            jsonSender->deleteIngredient(ingredient_id);
        });
        tableWidget->setCellWidget(i,3,deleteButton);
    }
}

void IngredientTable::setupContents()
{
    mainLayout->addWidget(tableWidget);
    mainLayout->setContentsMargins(0,0,0,0);
    tableWidget->setColumnCount(4);
    tableWidget->setFont(QFont("Sans serif",12));
    for(int i=0; i<tableWidget->columnCount(); i++)
        tableWidget->setColumnWidth(i,300);
    tableWidget->verticalHeader()->hide();
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Ингредиент"));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Срок годности"));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(""));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(""));
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setFocusPolicy(Qt::NoFocus);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setLayout(mainLayout);
}
