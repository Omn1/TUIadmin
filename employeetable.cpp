#include "employeetable.h"
#include "sessioninfo.h"
#include "employeeadderwidget.h"
#include <QHeaderView>
#include <QScrollBar>
#include <QPushButton>
#include <QFileDialog>

EmployeeTable::EmployeeTable(QWidget *parent, JsonDownloader *jsonLoader)
    : WrapperAwareWidget(parent)
    , mainLayout(new QVBoxLayout)
    , tableWidget(new QTableWidget)
    , loader(jsonLoader)
    , jsonSender(new JsonSender)
{
    if(!loader){
        loader = new JsonDownloader;
        connect(loader, &JsonDownloader::updateReady, this, &EmployeeTable::onNewEmployeeInfo);
        loader->start();
    }
    else{
        connect(loader, &JsonDownloader::updateReady, this, &EmployeeTable::onNewEmployeeInfo);
    }

    setupContents();
    onNewEmployeeInfo();
}

void EmployeeTable::fillWrapper()
{
    QPushButton *exportButton = new QPushButton("Экспортировать");
    connect(exportButton, &QPushButton::clicked, this, &EmployeeTable::exportAsCSV);
    exportButton->setMaximumWidth(150);
    wrapperLayout->addWidget(exportButton);

    QPushButton *addEmployeeButton = new QPushButton("Добавить сотрудника");
    connect(addEmployeeButton, &QPushButton::clicked, this, []{
       (new EmployeeAdderWidget)->show();
    });
    addEmployeeButton->setMaximumWidth(200);
    wrapperLayout->addWidget(addEmployeeButton);
}

void EmployeeTable::onNewEmployeeInfo()
{
    int scrollBarPosition = tableWidget->verticalScrollBar()->value();
    QJsonArray tableContent = loader->getPersonnelInfo();
    tableWidget->setRowCount(0);
    for(int item_i = 0; item_i < tableContent.size(); item_i++){
        QJsonObject json = tableContent[item_i].toObject();

        int employee_id = json["id"].toInt();
        int permissions = json["permissions"].toInt();

        int i = tableWidget->rowCount();
        tableWidget->insertRow(i);

        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(employee_id)));
        tableWidget->setItem(i, 1, new QTableWidgetItem(json["name"].toString()));
        tableWidget->setItem(i, 2, new QTableWidgetItem(json["login"].toString()));
        tableWidget->setItem(i, 3, new QTableWidgetItem(json["registered"].toString()));

        for(int j = 0; j < 6; j++) {
            tableWidget->setItem(i, j + 4, new QTableWidgetItem((permissions&(1<<j))?QString("Да"):QString("Нет")));
            tableWidget->item(i, j + 4)->setBackgroundColor((permissions&(1<<j))?QColor(0,255,0,127):QColor(255,0,0,127));
        }

        QPushButton *deleteThisButton = new QPushButton("Удалить");
        connect(deleteThisButton, &QPushButton::clicked, [employee_id,this](){
            jsonSender->deleteEmployee(employee_id);
        });
        tableWidget->setCellWidget(i,10,deleteThisButton);
        deleteThisButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::PERSONNEL_MGR_PERMISSION | permissions));
    }
    for(int j = 0; j < 4; j++) {
        tableWidget->resizeColumnToContents(j);
    }
    tableWidget->verticalScrollBar()->setValue(scrollBarPosition);
}

void EmployeeTable::exportAsCSV()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Экспортировать в CSV", "", "Comma-separated values (*.csv);;All files (*)");
    if(fileName.isEmpty())
        return;
    else{
        QFile csvFile(fileName);
        if(csvFile.open(QIODevice::WriteOnly)){
            QTextStream textStream (&csvFile);
            QStringList row;
            QJsonArray tableContent = loader->getPersonnelInfo();
            textStream << QString("ID;Имя;Логин;Дата регистрации;Повар;Водитель;Управление складом;Управление персоналом;Управление меню;Подтверждение заказов\n");
            for (int item_i = 0; item_i < tableContent.size(); item_i++) {
                row.clear();

                QJsonObject json = tableContent[item_i].toObject();

                int employee_id = json["id"].toInt();
                int permissions = json["permissions"].toInt();

                row << QString::number(employee_id) << json["name"].toString() << json["login"].toString() << json["registered"].toString();

                for(int j = 0; j < 6; j++) {
                    row << ((permissions&(1<<j))?QString("Да"):QString("Нет"));
                }

                textStream << row.join( ';' )+"\n";
            }
            csvFile.close();
        }
    }
}

void EmployeeTable::setupContents()
{
    mainLayout->addWidget(tableWidget);
    mainLayout->setContentsMargins(0,0,0,0);
    tableWidget->setColumnCount(11);
    tableWidget->setFont(QFont("Sans serif",12));
    for(int i = 4; i < tableWidget->columnCount(); i++){
        tableWidget->setColumnWidth(i, 180);
    }
    tableWidget->verticalHeader()->hide();
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("ID"));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Имя"));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Логин"));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Дата регистрации"));
    tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Повар"));
    tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Водитель"));
    tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("Управление складом"));
    tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("Управление персоналом"));
    tableWidget->setHorizontalHeaderItem(8, new QTableWidgetItem("Управление меню"));
    tableWidget->setHorizontalHeaderItem(9, new QTableWidgetItem("Подтверждение заказов"));
    tableWidget->setHorizontalHeaderItem(10, new QTableWidgetItem());
    for(int i = 0; i < 4; i++) {
        tableWidget->resizeColumnToContents(i);
    }
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setFocusPolicy(Qt::NoFocus);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setLayout(mainLayout);
}
