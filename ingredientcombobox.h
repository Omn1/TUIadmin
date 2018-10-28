#ifndef INGREDIENTCOMBOBOX_H
#define INGREDIENTCOMBOBOX_H

#include <QWidget>
#include <QComboBox>
#include "jsondownloader.h"

class IngredientComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit IngredientComboBox(QWidget *parent = nullptr, JsonDownloader *jsonLoader = nullptr);

signals:

public slots:
    void onNewIngredients();

private:
    JsonDownloader *loader;
};

#endif // INGREDIENTCOMBOBOX_H
