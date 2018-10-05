#include "ingredientadderwidget.h"
#include "ui_ingredientadderwidget.h"

IngredientAdderWidget::IngredientAdderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IngredientAdderWidget)
{
    ui->setupUi(this);
}

IngredientAdderWidget::~IngredientAdderWidget()
{
    delete ui;
}
