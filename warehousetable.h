#ifndef WAREHOUSETABLE_H
#define WAREHOUSETABLE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include "jsondownloader.h"
#include "jsonsender.h"

class WarehouseTable : public QWidget
{
    Q_OBJECT
public:
    explicit WarehouseTable(QWidget *parent = nullptr, JsonDownloader *jsonLoader = nullptr);

    void setDisplayedIngredientId(int value);

signals:

public slots:
    void onNewWarehouseInfo();
    void exportAsCSV();
private:
    void setupContents();
    QVBoxLayout *mainLayout;
    QTableWidget *tableWidget;
    JsonDownloader *loader;
    JsonSender *jsonSender;
    int displayed_ingredient_id;
};

#endif // WAREHOUSETABLE_H
