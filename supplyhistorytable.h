#ifndef SUPPLYHISTORYTABLE_H
#define SUPPLYHISTORYTABLE_H

#include <QTableWidget>
#include "wrapperawarewidget.h"
#include "jsondownloader.h"
#include "ingredientcombobox.h"

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
    IngredientComboBox *ingredientComboBox;
};

#endif // SUPPLYHISTORYTABLE_H
