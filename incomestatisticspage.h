#ifndef INCOMESTATISTICSPAGE_H
#define INCOMESTATISTICSPAGE_H

#include "wrapperawarewidget.h"
#include "statisticsengine.h"
#include <QSpinBox>

class IncomeStatisticsPage : public WrapperAwareWidget
{
    Q_OBJECT
public:
    explicit IncomeStatisticsPage(QWidget *parent = nullptr, StatisticsEngine *statEngine = nullptr);

    void setupContents();
    void fillWrapper() override;
signals:

public slots:
    void reloadChart(int days);
    void exportStats();
private:
    QChartView *chartView;
    StatisticsEngine *statEngine;
    QVBoxLayout *mainLayout;
    QSpinBox *spinBox;
};

#endif // INCOMESTATISTICSPAGE_H
