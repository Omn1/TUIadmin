#include "warehousetable.h"
#include "sessioninfo.h"
#include <QPushButton>
#include <QDate>
#include <QHeaderView>
#include <QFileDialog>
#include <QFile>
#include <QScrollBar>

WarehouseTable::WarehouseTable(QWidget *parent, JsonDownloader *jsonLoader)
    : WrapperAwareWidget(parent)
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
    ingredientComboBox = new IngredientComboBox(nullptr, loader);
    connect(ingredientComboBox, QOverload<int>::of(&IngredientComboBox::currentIndexChanged), this, &WarehouseTable::onNewWarehouseInfo);

    setupContents();
    setDisplayedIngredientId(0);
    onNewWarehouseInfo();
}

void WarehouseTable::setDisplayedIngredientId(int value)
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

void WarehouseTable::fillWrapper()
{
    wrapperLayout->addWidget(ingredientComboBox);
    QPushButton *exportButton = new QPushButton("Экспортировать");
    connect(exportButton, &QPushButton::clicked, this, &WarehouseTable::exportAsCSV);
    exportButton->setMaximumWidth(150);
    wrapperLayout->addWidget(exportButton);
}

void WarehouseTable::onNewWarehouseInfo()
{
    int scrollBarPosition = tableWidget->verticalScrollBar()->value();
    QJsonArray tableContent = loader->getWarehouseInfo();
    tableWidget->setRowCount(0);
    int displayed_ingredient_id = ingredientComboBox->currentData().toInt();
    for(int item_i = 0; item_i < tableContent.size(); item_i++){
        QJsonObject json = tableContent[item_i].toObject();
        int supply_id = json["id"].toInt();
        int ingredient_id = json["ingredient_id"].toInt();
        if (displayed_ingredient_id != 0 && ingredient_id != displayed_ingredient_id)
            continue;
        QJsonObject ingredientJson = loader->getIngredientById(ingredient_id);
        if(ingredientJson.empty())
            continue;
        int expiry_days = ingredientJson["expiry"].toInt();
        QString ingredient_title = ingredientJson["title"].toString();


        QString arrival_date_string = json["date"].toString();
        QStringList dmy = arrival_date_string.split("-");
        QDate arrival_date(dmy[2].toInt(), dmy[1].toInt(), dmy[0].toInt());
        QDate expiry_date = arrival_date;
        expiry_date = expiry_date.addDays(expiry_days);

        int expiry_minutes = expiry_days * 24 * 60;
        QDateTime now_date_time = QDateTime::currentDateTime();
        int passed_minutes = int((now_date_time.toSecsSinceEpoch() - QDateTime(arrival_date,QTime(0,0)).toSecsSinceEpoch())/60);

        double red_coef = std::min(1.0,passed_minutes*1.0/expiry_minutes);
        double green_coef = std::max(0.0,1.0 - passed_minutes*1.0/expiry_minutes);

        qDebug() << red_coef << " " << green_coef << "\n";

        QColor backgroundColor(int(255*red_coef), int(255*green_coef), 0, 127);

        int i = tableWidget->rowCount();
        tableWidget->insertRow(i);

        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(supply_id)));
        tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(ingredient_id)));
        tableWidget->setItem(i, 2, new QTableWidgetItem(ingredient_title));
        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(json["mass"].toDouble())));
        tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(expiry_days)));
        tableWidget->setItem(i, 5, new QTableWidgetItem(arrival_date_string));
        tableWidget->setItem(i, 6, new QTableWidgetItem(expiry_date.toString("dd-MM-yyyy")));

        QPushButton *deleteThisButton = new QPushButton("Удалить");
        connect(deleteThisButton, &QPushButton::clicked, [supply_id,this](){
            jsonSender->deleteSupply(supply_id);
        });
        tableWidget->setCellWidget(i,7,deleteThisButton);
        deleteThisButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::WAREHOUSE_MGR_PERMISSION));


        for (int j = 0; j < tableWidget->columnCount(); j++){
            if(tableWidget->item(i,j))
                tableWidget->item(i,j)->setBackgroundColor(backgroundColor);
        }
    }
    tableWidget->verticalScrollBar()->setValue(scrollBarPosition);
}

void WarehouseTable::exportAsCSV()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Экспортировать в CSV", "", "Comma-separated values (*.csv);;All files (*)");
    if(fileName.isEmpty())
        return;
    else{
        QFile csvFile(fileName);
        if(csvFile.open(QIODevice::WriteOnly)){
            QTextStream textStream (&csvFile);
            QStringList row;
            QJsonArray tableContent = loader->getWarehouseInfo();
            int displayed_ingredient_id = ingredientComboBox->currentData().toInt();
            textStream << QString("ID;ID ингредиента;Название;Количество;Срок годности;Дата поставки;Дата просрочки\n");
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
                int expiry_days = ingredientJson["expiry"].toInt();
                QString ingredient_title = ingredientJson["title"].toString();


                QString arrival_date_string = json["date"].toString();
                QStringList dmy = arrival_date_string.split("-");
                QDate arrival_date(dmy[2].toInt(), dmy[1].toInt(), dmy[0].toInt());
                QDate expiry_date = arrival_date;
                expiry_date = expiry_date.addDays(expiry_days);

                row << QString::number(supply_id) << QString::number(ingredient_id) << ingredient_title << QString::number(json["mass"].toDouble()) << QString::number(expiry_days) << arrival_date_string << expiry_date.toString("dd-MM-yyyy");
                textStream << row.join( ';' )+"\n";
            }
            csvFile.close();
        }
    }
}

void WarehouseTable::setupContents()
{
    mainLayout->addWidget(tableWidget);
    mainLayout->setContentsMargins(0,0,0,0);
    tableWidget->setColumnCount(8);
    tableWidget->setFont(QFont("Sans serif",12));
    for(int i = 0; i < tableWidget->columnCount(); i++){
        tableWidget->setColumnWidth(i, 170);
    }
    tableWidget->verticalHeader()->hide();
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("ID"));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("ID ингредиента"));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Название"));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Количество"));
    tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Срок годности (дни)"));
    tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Дата поставки"));
    tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("Дата просрочки"));
    tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem());
    //tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Конечный срок"));
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setFocusPolicy(Qt::NoFocus);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setLayout(mainLayout);
}
