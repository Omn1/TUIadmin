#ifndef INGREDIENTADDERWIDGET_H
#define INGREDIENTADDERWIDGET_H

#include <QWidget>

namespace Ui {
class IngredientAdderWidget;
}

class IngredientAdderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IngredientAdderWidget(QWidget *parent = nullptr);
    ~IngredientAdderWidget();

private:
    Ui::IngredientAdderWidget *ui;
};

#endif // INGREDIENTADDERWIDGET_H
