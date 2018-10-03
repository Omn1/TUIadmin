#ifndef INGREDIENTWIDGET_H
#define INGREDIENTWIDGET_H

#include <QWidget>
#include "jsondownloader.h"

namespace Ui {
class IngredientWidget;
}

class IngredientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IngredientWidget(QWidget *parent = nullptr);
    ~IngredientWidget();

    void setTitle(QString title);
    //void setMass(QString mass);
    void setImage(QPixmap pixmap);
    void loadFromJSON(const QJsonObject &json, JsonDownloader &loader);
private:
    Ui::IngredientWidget *ui;
};

#endif // INGREDIENTWIDGET_H
