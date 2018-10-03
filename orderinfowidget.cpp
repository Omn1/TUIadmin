#include "orderinfowidget.h"
#include "ui_orderinfowidget.h"

OrderInfoWidget::OrderInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderInfoWidget)
{
    ui->setupUi(this);
}

OrderInfoWidget::~OrderInfoWidget()
{
    delete ui;
}

void OrderInfoWidget::setID(QString id)
{
    ui->id_label->setText(id);
}

void OrderInfoWidget::setAddress(QString address)
{
    ui->address_label->setText(address);
}

void OrderInfoWidget::setPhone(QString phone)
{
    ui->phone_label->setText(phone);
}

void OrderInfoWidget::setName(QString name)
{
    ui->name_label->setText(name);
}
