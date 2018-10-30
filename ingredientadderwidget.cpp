#include "ingredientadderwidget.h"
#include "ui_ingredientadderwidget.h"

IngredientAdderWidget::IngredientAdderWidget(QWidget *parent, JsonDownloader *jsonLoader)
    : QWidget(parent)
    , ui(new Ui::IngredientAdderWidget)
    , loader(jsonLoader)
    , jsonSender(new JsonSender)
{
    ui->setupUi(this);

    if (jsonLoader == nullptr) {
        loader = new JsonDownloader;
        loader->start();
    }
    connect(ui->sendButton, &QPushButton::clicked, this, &IngredientAdderWidget::onSendButtonClicked);
}

IngredientAdderWidget::~IngredientAdderWidget()
{
    delete ui;
}

QJsonObject IngredientAdderWidget::getInfo()
{
    QJsonObject res;
    res["title"] = ui->nameEdit->text();
    res["expiry"] = QString::number(ui->daysSpinBox->value());
    return res;
}

void IngredientAdderWidget::onSendButtonClicked()
{
    jsonSender->addIngredent(getInfo());
}
