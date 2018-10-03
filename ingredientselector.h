#ifndef INGREDIENTSELECTOR_H
#define INGREDIENTSELECTOR_H

#include <QWidget>
#include "jsondownloader.h"

namespace Ui {
class IngredientSelector;
}

class IngredientSelector : public QWidget
{
    Q_OBJECT

public:
    explicit IngredientSelector(QWidget *parent = nullptr, JsonDownloader *jsonLoader = nullptr);
    ~IngredientSelector();

    QJsonObject getInfo();
public slots:
    void onIdEntered();
private:
    Ui::IngredientSelector *ui;
    JsonDownloader *loader;
};

#endif // INGREDIENTSELECTOR_H
