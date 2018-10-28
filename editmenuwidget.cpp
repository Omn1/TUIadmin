#include "editmenuwidget.h"
#include "ui_editmenuwidget.h"
#include "sessioninfo.h"

EditMenuWidget::EditMenuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditMenuWidget)
{
    ui->setupUi(this);
    addDishButton = ui->addDishButton;
    addDishButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::MENU_MGR_PERMISSION));
    addIngredientButton = ui->addIngredientButton;
    addIngredientButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::MENU_MGR_PERMISSION));
    checkDishesButton = ui->checkDishesButton;
    checkIngredientsButton = ui->checkIngredientsButton;
    uploadPhotoButton = ui->uploadPhotoButton;
    uploadPhotoButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::MENU_MGR_PERMISSION));
}

EditMenuWidget::~EditMenuWidget()
{
    delete ui;
}
