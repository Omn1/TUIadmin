#include "supplyhistorytable.h"
#include <QHeaderView>
#include <QScrollBar>
#include <QFileDialog>
#include <QPushButton>

SupplyHistoryTable::SupplyHistoryTable(QWidget *parent, JsonDownloader *jsonLoader)
    : WrapperAwareWidget(parent)
    , mainLayout(new QVBoxLayout)
    , tableWidget(new QTableWidget)
    , loader(jsonLoader)
{
    if(!loader){
        loader = new JsonDownloader;
        connect(loader, &JsonDownloader::updateReady, this, &SupplyHistoryTable::onNewSupplyHistoryInfo);
        loader->start();
    }
    else{
        connect(loader, &JsonDownloader::updateReady, this, &SupplyHistoryTable::onNewSupplyHistoryInfo);
    }
    ingredientComboBox = new IngredientComboBox(nullptr, loader);
    connect(ingredientComboBox, QOverload<int>::of(&IngredientComboBox::currentIndexChanged), this, &SupplyHistoryTable::onNewSupplyHistoryInfo);

    setupContents();
    setDisplayedIngredientId(0);
    onNewSupplyHistoryInfo();
}

void SupplyHistoryTable::setDisplayedIngredientId(int value)
{
    if (ingredientComboBox->currentData().toInt() != value)
    {
        for (int i = 0; i < ingredientComboBox->count(); i++) {
            if (ingredientComboBox->itemData(i).toInt() == value) {
                ingredientComboBox->setCurrentIndex(i);
                break;
            }
        }
    }
}

void SupplyHistoryTable::fillWrapper()
{
    wrapperLayout->addWidget(ingredientComboBox);
    QPushButton *exportButton = new QPushButton("Экспортировать");
    connect(exportButton, &QPushButton::clicked, this, &SupplyHistoryTable::exportAsCSV);
    exportButton->setMaximumWidth(150);
    wrapperLayout->addWidget(exportButton);
}

void SupplyHistoryTable::onNewSupplyHistoryInfo()
{
    int scrollBarPosition = tableWidget->verticalScrollBar()->value();
    QJsonArray tableContent = loader->getSupplyHistory();
    int displayed_ingredient_id = ingredientComboBox->currentData().toInt();
    tableWidget->setRowCount(0);
    for(int item_i = 0; item_i < tableContent.size(); item_i++){
        QJsonObject json = tableContent[item_i].toObject();
        int supply_id = json["id"].toInt();
        int ingredient_id = json["ingredient_id"].toInt();
        if (displayed_ingredient_id != 0 && ingredient_id != displayed_ingredient_id)
            continue;
        QJsonObject ingredientJson = loader->getIngredientById(ingredient_id);
        if(ingredientJson.empty())
            continue;

        int i = tableWidget->rowCount();
        tableWidget->insertRow(i);

        QString action;
        int action_id = json["action"].toInt();
        if (action_id == 0) action = "Поставлено";
        else if (action_id == 1) action = "Утилизировано";
        else action = "Использовано";

        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(supply_id)));
        tableWidget->setItem(i, 1, new QTableWidgetItem(json["date"].toString()));
        tableWidget->setItem(i, 2, new QTableWidgetItem(json["time"].toString()));
        tableWidget->setItem(i, 3, new QTableWidgetItem(action));
        tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(ingredient_id)));
        tableWidget->setItem(i, 5, new QTableWidgetItem(ingredientJson["title"].toString()));
        tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(json["mass"].toDouble())));
    }
    tableWidget->verticalScrollBar()->setValue(scrollBarPosition);
}

void SupplyHistoryTable::exportAsCSV()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Экспортировать в CSV", "", "Comma-separated values (*.csv);;All files (*)");
    if(fileName.isEmpty())
        return;
    else{
        QFile csvFile(fileName);
        if(csvFile.open(QIODevice::WriteOnly)){
            QTextStream textStream (&csvFile);
            QStringList row;
            QJsonArray tableContent = loader->getSupplyHistory();
            int displayed_ingredient_id = ingredientComboBox->currentData().toInt();
            textStream << QString("ID;Дата;Время;Действие;ID ингредиента;Название;Количество\n");
            for (int item_i = 0; item_i < tableContent.size(); item_i++) {
                row.clear();

                QJsonObject json = tableContent[item_i].toObject();
                int supply_id = json["id"].toInt();
                int ingredient_id = json["ingredient_id"].toInt();
                if (displayed_ingredient_id != 0 && ingredient_id != displayed_ingredient_id)
                    continue;
                QJsonObject ingredientJson = loader->getIngredientById(ingredient_id);
                if(ingredientJson.empty())
                    continue;
                QString action;
                int action_id = json["action"].toInt();
                if (action_id == 0) action = "Поставлено";
                else if (action_id == 1) action = "Утилизировано";
                else action = "Использовано";

                row << QString::number(supply_id) << json["date"].toString() << json["time"].toString() << action << QString::number(ingredient_id) << ingredientJson["title"].toString() << QString::number(json["mass"].toDouble());
                textStream << row.join( ';' )+"\n";
            }
            csvFile.close();
        }
    }
}

void SupplyHistoryTable::setupContents()
{
    mainLayout->addWidget(tableWidget);
    mainLayout->setContentsMargins(0,0,0,0);
    tableWidget->setColumnCount(7);
    tableWidget->setFont(QFont("Sans serif",12));
    for(int i = 0; i < tableWidget->columnCount(); i++){
        tableWidget->setColumnWidth(i, 170);
    }
    tableWidget->verticalHeader()->hide();
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("ID"));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Дата"));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Время"));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Действие"));
    tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("ID ингредиента"));
    tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Название"));
    tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("Количество"));
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setFocusPolicy(Qt::NoFocus);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setLayout(mainLayout);
}
