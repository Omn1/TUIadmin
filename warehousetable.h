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

signals:

public slots:
    void onNewWarehouseInfo();
private:
    void setupContents();
    QVBoxLayout *mainLayout;
    QTableWidget *tableWidget;
    JsonDownloader *loader;
    JsonSender *jsonSender;
};

#endif // WAREHOUSETABLE_H
