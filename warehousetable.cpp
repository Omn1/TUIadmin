#include "warehousetable.h"

WarehouseTable::WarehouseTable(QWidget *parent)
    : QWidget(parent)
    , mainLayout(new QVBoxLayout)
    , tableWidget(new QTableWidget)
{
    setupContents();
}

void WarehouseTable::setupContents()
{
    mainLayout->addWidget(tableWidget);
    tableWidget->setRowCount(5);
    tableWidget->setColumnCount(6);
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("ID"));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Название"));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Количество"));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Срок годности"));
    tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Дата поставки"));
    tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Конечный срок"));
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setFocusPolicy(Qt::NoFocus);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setLayout(mainLayout);
}
