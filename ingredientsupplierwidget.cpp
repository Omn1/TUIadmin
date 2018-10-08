#include "ingredientsupplierwidget.h"
#include "ui_ingredientsupplierwidget.h"

IngredientSupplierWidget::IngredientSupplierWidget(QWidget *parent, JsonDownloader *jsonLoader)
    : QWidget(parent)
    , ui(new Ui::IngredientSupplierWidget)
    , loader(jsonLoader)
    , jsonSender(new JsonSender)
{
    ui->setupUi(this);

    if (!loader)
    {
        loader = new JsonDownloader;
        selector = new IngredientSelector(nullptr, loader);
        loader->start();
    }
    else
    {
        selector = new IngredientSelector(nullptr, loader);
    }

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(selector);
    ui->frame->setContentsMargins(0,0,0,0);
    vlayout->setContentsMargins(0,0,0,0);
    ui->frame->setLayout(vlayout);

    connect(ui->pushButton, &QPushButton::clicked, this, &IngredientSupplierWidget::onSendButtonClicked);
}

IngredientSupplierWidget::~IngredientSupplierWidget()
{
    delete ui;
}

void IngredientSupplierWidget::onSendButtonClicked()
{
    jsonSender->supplyIngredient(selector->getInfo());
}
