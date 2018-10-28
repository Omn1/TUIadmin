#include "mainmenuwidget.h"
#include "ui_mainmenuwidget.h"

MainMenuWidget::MainMenuWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainMenuWidget)
{
    ui->setupUi(this);
    checkOrdersButton = ui->checkOrdersButton;
    editMenuButton = ui->editMenuButton;
    checkWarehouseButton = ui->checkWarehouseButton;
    supplyButton = ui->supplyButton;
    supplyHistoryButton = ui->supplyHistoryButton;
    statsButton = ui->statsButton;
    personnelButton = ui->personnelButton;
}

MainMenuWidget::~MainMenuWidget()
{
    delete ui;
}
