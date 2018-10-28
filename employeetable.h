#ifndef EMPLOYEETABLE_H
#define EMPLOYEETABLE_H

#include <QTableWidget>
#include "wrapperawarewidget.h"
#include "jsondownloader.h"
#include "jsonsender.h"
#include "ingredientcombobox.h"


class EmployeeTable : public WrapperAwareWidget
{
    Q_OBJECT
public:
    explicit EmployeeTable(QWidget *parent = nullptr, JsonDownloader *jsonLoader = nullptr);

    void fillWrapper() override;
signals:

public slots:
    void onNewEmployeeInfo();
    void exportAsCSV();
private:
    void setupContents();
    QVBoxLayout *mainLayout;
    QTableWidget *tableWidget;
    JsonDownloader *loader;
    JsonSender *jsonSender;
};

#endif // EMPLOYEETABLE_H
