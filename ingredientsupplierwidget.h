#ifndef INGREDIENTSUPPLIERWIDGET_H
#define INGREDIENTSUPPLIERWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include "jsondownloader.h"
#include "jsonsender.h"
#include "ingredientselector.h"

namespace Ui {
class IngredientSupplierWidget;
}

class IngredientSupplierWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IngredientSupplierWidget(QWidget *parent = nullptr, JsonDownloader *jsonLoader = nullptr);
    ~IngredientSupplierWidget();

public slots:
    void onSendButtonClicked();

private:
    Ui::IngredientSupplierWidget *ui;
    JsonDownloader *loader;
    JsonSender *jsonSender;
    IngredientSelector *selector;
};

#endif // INGREDIENTSUPPLIERWIDGET_H
