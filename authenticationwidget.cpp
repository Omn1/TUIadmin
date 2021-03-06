#include "authenticationwidget.h"
#include "ui_authenticationwidget.h"
#include "sessioninfo.h"

AuthenticationWidget::AuthenticationWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AuthenticationWidget)
    , jsonSender(new JsonSender)
    , cafeJsonSender(new JsonSender)
{
    ui->setupUi(this);
    connect(jsonSender, &JsonSender::jsonSent, this, &AuthenticationWidget::onLoggedIn);
    connect(ui->pushButton, &QPushButton::clicked, this, &AuthenticationWidget::onLoginButtonPressed);
    connect(ui->serverEdit, &QLineEdit::textChanged, this, &AuthenticationWidget::getCafeList);
    connect(cafeJsonSender, &JsonSender::jsonSent, this, &AuthenticationWidget::onGotCafeList);
    getCafeList();
}

AuthenticationWidget::~AuthenticationWidget()
{
    delete ui;
}

void AuthenticationWidget::onLoginButtonPressed()
{
    SessionInfo::APIurl = ui->serverEdit->text();
    jsonSender->authenticate(ui->loginEdit->text(), ui->passwordEdit->text(), ui->comboBox->currentData().toInt());
}

void AuthenticationWidget::onLoggedIn(bool success)
{
    if (success) {
        QString login = jsonSender->lastAnswer["res"].toObject()["login"].toString();
        QString token = jsonSender->lastAnswer["res"].toObject()["token"].toString();
        int permissions = jsonSender->lastAnswer["res"].toObject()["permissions"].toInt();
        SessionInfo::loginInfo = "login="+login+"&token="+token;
        SessionInfo::permissions = permissions;
        emit authenticationPassed();
    }
}

void AuthenticationWidget::onGotCafeList(bool success)
{
    if(success) {
        QJsonArray cafesList = cafeJsonSender->lastAnswer["res"].toArray();
        ui->comboBox->clear();
        for (int i = 0; i < cafesList.size(); i++) {
            QJsonObject json = cafesList[i].toObject();
            ui->comboBox->addItem(json["title"].toString(), json["id"].toInt());
        }
    }
}

void AuthenticationWidget::getCafeList()
{
    SessionInfo::APIurl = ui->serverEdit->text();
    cafeJsonSender->getCafeList();
}
