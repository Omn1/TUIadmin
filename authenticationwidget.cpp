#include "authenticationwidget.h"
#include "ui_authenticationwidget.h"

AuthenticationWidget::AuthenticationWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AuthenticationWidget)
    , jsonSender(new JsonSender)
{
    ui->setupUi(this);
    connect(jsonSender, &JsonSender::jsonSent, this, &AuthenticationWidget::onLoggedIn);
    connect(ui->pushButton, &QPushButton::clicked, this, &AuthenticationWidget::onLoginButtonPressed);
}

AuthenticationWidget::~AuthenticationWidget()
{
    delete ui;
}

void AuthenticationWidget::onLoginButtonPressed()
{
    JsonSender::APIurl = ui->serverEdit->text();
    JsonDownloader::APIurl = ui->serverEdit->text();
    jsonSender->authenticate(ui->loginEdit->text(), ui->passwordEdit->text());
}

void AuthenticationWidget::onLoggedIn(bool success)
{
    if (success) {
        QString login = jsonSender->lastAnswer["res"].toObject()["login"].toString();
        QString token = jsonSender->lastAnswer["res"].toObject()["token"].toString();
        JsonSender::loginInfo = JsonDownloader::loginInfo = "login="+login+"&token="+token;
        emit authenticationPassed();
    }
}
