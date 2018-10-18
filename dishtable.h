#ifndef DISHTABLE_H
#define DISHTABLE_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QTreeWidget>
#include <QHeaderView>
#include <QJsonObject>
#include <QJsonArray>
#include <QPushButton>
#include "orderwidget.h"
#include "dishwidget.h"
#include "jsondownloader.h"
#include "statisticsengine.h"

class DishTable : public QWidget
{
    Q_OBJECT
public:
    explicit DishTable(QWidget *parent = nullptr, JsonDownloader *loader = nullptr);

    void reloadDishes();
signals:

public slots:
    void onNewDishes();
private:
    QVBoxLayout *mainLayout;
    QTreeWidget *treeWidget;
    JsonDownloader *loader;
    StatisticsEngine *statEngine;
    QJsonArray dishes;
};

#endif // DISHTABLE_H
