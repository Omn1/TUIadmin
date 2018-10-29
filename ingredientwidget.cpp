#include "ingredientwidget.h"
#include "ui_ingredientwidget.h"

IngredientWidget::IngredientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IngredientWidget)
{
    ui->setupUi(this);
}

IngredientWidget::~IngredientWidget()
{
    delete ui;
}

void IngredientWidget::setTitle(QString title)
{
    ui->titleLabel->setText(title);
}

/*void IngredientWidget::setMass(QString mass)
{
    ui->massLabel->setText(mass);
}*/

void IngredientWidget::setImage(QPixmap pixmap)
{
    //ui->imageLabel->setPixmap(pixmap);
}

void IngredientWidget::loadFromJSON(const QJsonObject &json, JsonDownloader &loader)
{
    setTitle(loader.getIngredientById(json["id"].toInt())["title"].toString());
}
