#include "incomestatisticspage.h"
#include <QLabel>
#include <QPushButton>
IncomeStatisticsPage::IncomeStatisticsPage(QWidget *parent, StatisticsEngine *statEngine)
    : WrapperAwareWidget(parent)
    , statEngine(statEngine)
    , mainLayout(new QVBoxLayout)
    , spinBox(new QSpinBox)
{
    if (!statEngine) {
        statEngine = new StatisticsEngine;
    }
    setupContents();
}

void IncomeStatisticsPage::setupContents()
{
    chartView = new QChartView(statEngine->getRecentIncomeChart(7));
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(800,600);
    mainLayout->addWidget(chartView);
    setLayout(mainLayout);
}

void IncomeStatisticsPage::fillWrapper()
{
    QLabel *spinBoxLabel = new QLabel("Количетсво дней:");
    wrapperLayout->addWidget(spinBoxLabel);
    spinBox->setMinimum(1);
    spinBox->setMaximum(31);
    spinBox->setValue(7);
    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &IncomeStatisticsPage::reloadChart);
    wrapperLayout->addWidget(spinBox);

    QPushButton *exportButton = new QPushButton("Экспортировать");
    exportButton->setMaximumWidth(150);
    connect(exportButton, &QPushButton::clicked, this, &IncomeStatisticsPage::exportStats);
    wrapperLayout->addWidget(exportButton);
}

void IncomeStatisticsPage::reloadChart(int days)
{
    delete chartView;
    chartView = new QChartView(statEngine->getRecentIncomeChart(days));
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(800,600);
    mainLayout->addWidget(chartView);
}

void IncomeStatisticsPage::exportStats()
{
    statEngine->exportRecentIncomeStatsAsCSV(spinBox->value());
}
