#include "editmenuwidget.h"
#include "ui_editmenuwidget.h"

EditMenuWidget::EditMenuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditMenuWidget)
{
    ui->setupUi(this);
    addDishButton = ui->addDishButton;
    addIngredientButton = ui->addIngredientButton;
}

EditMenuWidget::~EditMenuWidget()
{
    delete ui;
}
