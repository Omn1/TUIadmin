#ifndef WAREHOUSETABLE_H
#define WAREHOUSETABLE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>

class WarehouseTable : public QWidget
{
    Q_OBJECT
public:
    explicit WarehouseTable(QWidget *parent = nullptr);

signals:

public slots:

private:
    void setupContents();
    QVBoxLayout *mainLayout;
    QTableWidget *tableWidget;
};

#endif // WAREHOUSETABLE_H
