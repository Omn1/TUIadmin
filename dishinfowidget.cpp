#include "dishinfowidget.h"
#include "ui_dishinfowidget.h"

DishInfoWidget::DishInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DishInfoWidget)
{
    ui->setupUi(this);
}

DishInfoWidget::~DishInfoWidget()
{
    delete ui;
}

void DishInfoWidget::setTitle(QString title)
{
    ui->nameLabel->setText(title);
}

void DishInfoWidget::setPrice(QString price)
{
    ui->price_label->setText(price);
}

void DishInfoWidget::setMass(QString mass)
{
    ui->mass_label->setText(mass);
}

void DishInfoWidget::setImage(QPixmap pixmap)
{
    ui->imageLabel->setPixmap(pixmap);
}
