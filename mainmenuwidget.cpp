#include "mainmenuwidget.h"
#include "ui_mainmenuwidget.h"
#include "sessioninfo.h"

MainMenuWidget::MainMenuWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainMenuWidget)
{
    ui->setupUi(this);
    checkOrdersButton = ui->checkOrdersButton;
    editMenuButton = ui->editMenuButton;
    checkWarehouseButton = ui->checkWarehouseButton;
    supplyButton = ui->supplyButton;
    supplyButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::WAREHOUSE_MGR_PERMISSION));
    supplyHistoryButton = ui->supplyHistoryButton;
    statsButton = ui->statsButton;
    personnelButton = ui->personnelButton;
    personnelButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::PERSONNEL_MGR_PERMISSION));
}

MainMenuWidget::~MainMenuWidget()
{
    delete ui;
}
