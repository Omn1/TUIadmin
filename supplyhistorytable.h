#ifndef SUPPLYHISTORYTABLE_H
#define SUPPLYHISTORYTABLE_H

#include <QTableWidget>
#include "wrapperawarewidget.h"
#include "jsondownloader.h"

class SupplyHistoryTable : public WrapperAwareWidget
{
    Q_OBJECT
public:
    explicit SupplyHistoryTable(QWidget *parent = nullptr, JsonDownloader *jsonLoader = nullptr);

    void setDisplayedIngredientId(int value);

    void fillWrapper() override;
signals:

public slots:
    void onNewSupplyHistoryInfo();
    void exportAsCSV();
private:
    void setupContents();
    QVBoxLayout *mainLayout;
    QTableWidget *tableWidget;
    JsonDownloader *loader;
    int displayed_ingredient_id;
};

#endif // SUPPLYHISTORYTABLE_H
