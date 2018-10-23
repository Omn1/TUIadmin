#include "editmenuwidget.h"
#include "ui_editmenuwidget.h"

EditMenuWidget::EditMenuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditMenuWidget)
{
    ui->setupUi(this);
    addDishButton = ui->addDishButton;
    addIngredientButton = ui->addIngredientButton;
    checkDishesButton = ui->checkDishesButton;
    checkIngredientsButton = ui->checkIngredientsButton;
}

EditMenuWidget::~EditMenuWidget()
{
    delete ui;
}
