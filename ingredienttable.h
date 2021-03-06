#ifndef INGREDIENTTABLE_H
#define INGREDIENTTABLE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include "jsondownloader.h"
#include "jsonsender.h"
#include "statisticsengine.h"

class IngredientTable : public QWidget
{
    Q_OBJECT
public:
    explicit IngredientTable(QWidget *parent = nullptr, JsonDownloader *jsonLoader = nullptr);

signals:
    void checkWarehouse(int);
    void checkStatsTable(int);
public slots:
    void onNewIngredients();
private:
    void setupContents();
    QVBoxLayout *mainLayout;
    QTableWidget *tableWidget;
    JsonDownloader *loader;
    JsonSender *jsonSender;
    StatisticsEngine *statEngine;
};

#endif // INGREDIENTTABLE_H
