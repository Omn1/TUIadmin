#include "ingredienttable.h"
#include <QHeaderView>
#include <QPushButton>
#include "ingredientwidget.h"
#include "sessioninfo.h"

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
    statEngine = new StatisticsEngine(nullptr, loader);

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

        tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(json["expiry"].toInt())));

        int ingredient_id = json["id"].toInt();

        QPushButton *warehouseButton = new QPushButton("Посмотреть на складе");
        connect(warehouseButton, &QPushButton::clicked, [this,ingredient_id]{
            emit checkWarehouse(ingredient_id);
        });
        tableWidget->setCellWidget(i,2,warehouseButton);

        QPushButton *statsTableButton = new QPushButton("Смотреть отчет (таблица)");
        statsTableButton->setFont(QFont("Sans serif",12));
        connect(statsTableButton, &QPushButton::clicked, [this,ingredient_id]{
            emit checkStatsTable(ingredient_id);
        });
        tableWidget->setCellWidget(i,3,statsTableButton);

        QPushButton *statsChartButton = new QPushButton("Смотреть отчет (график)");
        statsChartButton->setFont(QFont("Sans serif",12));
        connect(statsChartButton, &QPushButton::clicked, [this,ingredient_id]{
            QChartView *cv = new QChartView(statEngine->getRecentIngredientStatsChart(ingredient_id,7));
            cv->setRenderHint(QPainter::Antialiasing);
            cv->setMinimumSize(800,600);
            cv->show();
        });
        tableWidget->setCellWidget(i,4,statsChartButton);

        QPushButton *deleteButton = new QPushButton("Удалить");
        connect(deleteButton, &QPushButton::clicked, [this,ingredient_id]{
            jsonSender->deleteIngredient(ingredient_id);
        });
        tableWidget->setCellWidget(i,5,deleteButton);
        deleteButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::MENU_MGR_PERMISSION));
    }
}

void IngredientTable::setupContents()
{
    mainLayout->addWidget(tableWidget);
    mainLayout->setContentsMargins(0,0,0,0);
    tableWidget->setColumnCount(6);
    tableWidget->setFont(QFont("Sans serif",12));
    for(int i=0; i<tableWidget->columnCount(); i++)
        tableWidget->setColumnWidth(i,300);
    tableWidget->verticalHeader()->hide();
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Ингредиент"));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Срок годности"));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(""));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(""));
    tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem(""));
    tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem(""));
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setFocusPolicy(Qt::NoFocus);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setLayout(mainLayout);
}
