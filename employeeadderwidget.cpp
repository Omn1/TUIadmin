#include "employeeadderwidget.h"
#include "ui_employeeadderwidget.h"
#include "sessioninfo.h"

EmployeeAdderWidget::EmployeeAdderWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EmployeeAdderWidget)
    , jsonSender(new JsonSender)
{
    ui->setupUi(this);

    setInitialPermissions();
    updateColors();

    connect(ui->cookButton, &QPushButton::clicked, this, &EmployeeAdderWidget::onPermissionButtonClicked);
    connect(ui->driverButton, &QPushButton::clicked, this, &EmployeeAdderWidget::onPermissionButtonClicked);
    connect(ui->wrhMgrButton, &QPushButton::clicked, this, &EmployeeAdderWidget::onPermissionButtonClicked);
    connect(ui->personnelMgrButton, &QPushButton::clicked, this, &EmployeeAdderWidget::onPermissionButtonClicked);
    connect(ui->menuMgrButton, &QPushButton::clicked, this, &EmployeeAdderWidget::onPermissionButtonClicked);
    connect(ui->operatorButton, &QPushButton::clicked, this, &EmployeeAdderWidget::onPermissionButtonClicked);

    connect(ui->sendButton, &QPushButton::clicked, this, &EmployeeAdderWidget::onSendButtonClicked);
}

EmployeeAdderWidget::~EmployeeAdderWidget()
{
    delete ui;
}

void EmployeeAdderWidget::setInitialPermissions()
{
    ui->cookButton->setText(SessionInfo::checkPermissions(SessionInfo::COOK_PERMISSION) ? QString("Да") : QString("Нет"));
    ui->driverButton->setText(SessionInfo::checkPermissions(SessionInfo::DRIVER_PERMISSION) ? QString("Да") : QString("Нет"));
    ui->wrhMgrButton->setText(SessionInfo::checkPermissions(SessionInfo::WAREHOUSE_MGR_PERMISSION) ? QString("Да") : QString("Нет"));
    ui->personnelMgrButton->setText(SessionInfo::checkPermissions(SessionInfo::PERSONNEL_MGR_PERMISSION) ? QString("Да") : QString("Нет"));
    ui->menuMgrButton->setText(SessionInfo::checkPermissions(SessionInfo::MENU_MGR_PERMISSION) ? QString("Да") : QString("Нет"));
    ui->operatorButton->setText(SessionInfo::checkPermissions(SessionInfo::OPERATOR_PERMISSION) ? QString("Да") : QString("Нет"));

    ui->cookButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::COOK_PERMISSION));
    ui->driverButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::DRIVER_PERMISSION));
    ui->wrhMgrButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::WAREHOUSE_MGR_PERMISSION));
    ui->personnelMgrButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::PERSONNEL_MGR_PERMISSION));
    ui->menuMgrButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::MENU_MGR_PERMISSION));
    ui->operatorButton->setEnabled(SessionInfo::checkPermissions(SessionInfo::OPERATOR_PERMISSION));
}

void EmployeeAdderWidget::updateColors()
{
    updateColor(ui->cookButton);
    updateColor(ui->driverButton);
    updateColor(ui->wrhMgrButton);
    updateColor(ui->personnelMgrButton);
    updateColor(ui->menuMgrButton);
    updateColor(ui->operatorButton);
}

void EmployeeAdderWidget::updateColor(QPushButton *button)
{
    QPalette pal = button->palette();
    pal.setColor(QPalette::Button, button->text() == QString("Да") ? QColor(0,255,0,127) : QColor(255,0,0,127));
    button->setAutoFillBackground(true);
    button->setPalette(pal);
    button->update();
}

QJsonObject EmployeeAdderWidget::getInfo()
{
    QJsonObject res;
    res["name"] = ui->nameEdit->text();
    res["login"] = ui->loginEdit->text();
    res["password"] = ui->passwordEdit->text();
    int permissions = 0;
    if (ui->cookButton->text() == QString("Да")) permissions |= 1 << 0;
    if (ui->driverButton->text() == QString("Да")) permissions |= 1 << 1;
    if (ui->wrhMgrButton->text() == QString("Да")) permissions |= 1 << 2;
    if (ui->personnelMgrButton->text() == QString("Да")) permissions |= 1 << 3;
    if (ui->menuMgrButton->text() == QString("Да")) permissions |= 1 << 4;
    if (ui->operatorButton->text() == QString("Да")) permissions |= 1 << 5;
    res["permission"] = permissions;
    return res;
}

void EmployeeAdderWidget::onPermissionButtonClicked()
{
    QPushButton * button = dynamic_cast<QPushButton*>(sender());
    if(!button)return;
    if (button->text() == QString("Да")) {
        button->setText(QString("Нет"));
    }
    else {
        button->setText(QString("Да"));
    }
    updateColors();
}

void EmployeeAdderWidget::onSendButtonClicked()
{
    jsonSender->addEmployee(getInfo());
}
