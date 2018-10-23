#ifndef STATISTICSENGINE_H
#define STATISTICSENGINE_H

#include <QObject>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include "jsondownloader.h"

QT_CHARTS_USE_NAMESPACE

class StatisticsEngine : public QObject
{
    Q_OBJECT
public:
    explicit StatisticsEngine(QObject *parent = nullptr, JsonDownloader *jsonLoader = nullptr);
    ~StatisticsEngine();

    QChart *getRecentDishOrdersChart(int id, int n_days);
    QChart *getRecentIncomeChart(int n_days);

    int getDishOrdersByDate(int id, QString date);
    double getIncomeByDate(QString date);
signals:

public slots:

private:
    JsonDownloader *loader;
};

#endif // STATISTICSENGINE_H
