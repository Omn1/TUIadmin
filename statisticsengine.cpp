#include "statisticsengine.h"
#include <QFileDialog>

StatisticsEngine::StatisticsEngine(QObject *parent, JsonDownloader *jsonLoader)
    : QObject(parent)
    , loader(jsonLoader)
{
    if(!loader){
        loader = new JsonDownloader;
        loader->start();
    }

}

StatisticsEngine::~StatisticsEngine()
{

}

QChart *StatisticsEngine::getRecentDishOrdersChart(int id, int n_days)
{
    QDate cur_date = QDate::currentDate();
    cur_date = cur_date.addDays(-n_days);
    QBarSet *barSet = new QBarSet("Количество заказов за день");
    QStringList categories;
    for (int i = 0; i < n_days; i++) {
        cur_date = cur_date.addDays(1);
        categories << cur_date.toString("dd.MM.yy");
        *barSet << getDishOrdersByDate(id, cur_date.toString("dd-MM-yyyy"));
    }
    QBarSeries *series = new QBarSeries;
    series->append(barSet);
    QChart *chart = new QChart;
    chart->addSeries(series);
    chart->setTitle("Статистика блюда");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    return chart;
}

QChart *StatisticsEngine::getRecentIncomeChart(int n_days)
{
    QDate cur_date = QDate::currentDate();
    cur_date = cur_date.addDays(-n_days);
    QBarSet *barSet = new QBarSet("Прибыль за день");
    QStringList categories;
    for (int i = 0; i < n_days; i++) {
        cur_date = cur_date.addDays(1);
        categories << cur_date.toString("dd.MM.yy");
        *barSet << getIncomeByDate(cur_date.toString("dd-MM-yyyy"));
    }
    QBarSeries *series = new QBarSeries;
    series->append(barSet);
    QChart *chart = new QChart;
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    return chart;
}

QChart *StatisticsEngine::getRecentIngredientStatsChart(int id, int n_days)
{
    QDate cur_date = QDate::currentDate();
    cur_date = cur_date.addDays(-n_days);
    QBarSet *arrivedSet = new QBarSet("Поставлено");
    QBarSet *utilizedSet = new QBarSet("Утилизировано");
    QBarSet *usedSet = new QBarSet("Использовано");
    QStringList categories;
    for (int i = 0; i < n_days; i++) {
        cur_date = cur_date.addDays(1);
        categories << cur_date.toString("dd.MM.yy");
        *arrivedSet << getIngredientAmountByDate(id, 0, cur_date.toString("dd-MM-yyyy"));
        *utilizedSet << getIngredientAmountByDate(id, 1, cur_date.toString("dd-MM-yyyy"));
        *usedSet << getIngredientAmountByDate(id, 2, cur_date.toString("dd-MM-yyyy"));
    }
    QBarSeries *series = new QBarSeries;
    series->append(arrivedSet);
    series->append(usedSet);
    series->append(utilizedSet);
    QChart *chart = new QChart;
    chart->addSeries(series);
    chart->setTitle("Отчет по ингредиенту");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    return chart;
}

void StatisticsEngine::exportRecentIncomeStatsAsCSV(int n_days)
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Экспортироваь в CSV", "", "Comma-separated values (*.csv);;All files (*)");
    if(fileName.isEmpty())
        return;
    else{
        QFile csvFile(fileName);
        if(csvFile.open(QIODevice::WriteOnly)){
            QTextStream textStream (&csvFile);
            QStringList row;
            QDate cur_date = QDate::currentDate();
            cur_date = cur_date.addDays(-n_days);
            textStream << QString("Дата;Прибыль\n");
            for (int i = 0; i < n_days; i++) {
                row.clear();
                cur_date = cur_date.addDays(1);
                row << cur_date.toString("dd.MM.yy") << QString::number(getIncomeByDate(cur_date.toString("dd-MM-yyyy")));
                textStream << row.join(';')+"\n";
            }
            csvFile.close();
        }
    }
}

int StatisticsEngine::getDishOrdersByDate(int id, QString date)
{
    int cou = 0;
    QJsonArray orders = loader->getOrders();
    for (int i = 0; i < orders.size(); i++) {
        if (orders[i].toObject()["date"].toString() != date)
            continue;
        QJsonArray curDishes = orders[i].toObject()["dishes"].toArray();
        for (int j = 0; j < curDishes.size(); j++) {
            QJsonObject dish = curDishes[j].toObject();
            if (dish["id"].toInt() == id)
                cou += dish["number"].toInt();
        }
    }
    return cou;
}

double StatisticsEngine::getIncomeByDate(QString date)
{
    double income = 0;
    QJsonArray orders = loader->getOrders();
    for (int i = 0; i < orders.size(); i++) {
        if (orders[i].toObject()["date"].toString() != date)
            continue;
        QJsonArray curDishes = orders[i].toObject()["dishes"].toArray();
        for (int j = 0; j < curDishes.size(); j++) {
            QJsonObject dish = curDishes[j].toObject();
            income += loader->getDishById(dish["id"].toInt())["cost"].toDouble() * dish["number"].toInt();
        }
    }
    return income;
}

double StatisticsEngine::getIngredientAmountByDate(int id, int action, QString date)
{
    double mass = 0;
    QJsonArray supplyHistory = loader->getSupplyHistory();
    for (int i = 0; i < supplyHistory.size(); i++) {
        QJsonObject json = supplyHistory[i].toObject();
        if (json["action"].toInt() == action && json["date"].toString() == date && (id == -1 || id == json["ingredient_id"].toInt())) {
            mass += json["mass"].toDouble();
        }
    }
    return mass;
}
