#include "dishadderwidget.h"
#include "ui_dishadderwidget.h"

DishAdderWidget::DishAdderWidget(QWidget *parent, JsonDownloader *jsonLoader)
    : QWidget(parent)
    , ui(new Ui::DishAdderWidget)
    , loader(jsonLoader)
    , jsonSender(new JsonSender)
    , addButton(new QPushButton("Добавить"))
{
    ui->setupUi(this);
    ui->tagsEdit->setValidator(new QRegExpValidator(QRegExp("(([a-z]+ )*([a-z]+),)*(([a-z]+ )*([a-z]+))")));

    if (jsonLoader == nullptr) {
        loader = new JsonDownloader;
        loader->start();
    }

    imageSelector = new ImageSelector(nullptr, jsonLoader);
    ui->gridLayout->addWidget(imageSelector, 5, 2, 1, 2);

    ui->tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableWidget->setCellWidget(0,1,addButton);
    ui->tableWidget->setColumnWidth(0,500);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    connect(addButton, &QPushButton::clicked, this, &DishAdderWidget::onAddButtonClicked);
    connect(ui->sendButton, &QPushButton::clicked, this, &DishAdderWidget::onSendButtonClicked);
}

DishAdderWidget::~DishAdderWidget()
{
    delete ui;
}

void DishAdderWidget::onAddButtonClicked()
{
    int new_row = ui->tableWidget->rowCount() - 1;
    ui->tableWidget->insertRow(new_row);
    IngredientSelector *newSelector = new IngredientSelector(nullptr, loader);
    ui->tableWidget->setRowHeight(new_row, newSelector->height());
    ui->tableWidget->setCellWidget(new_row, 0, newSelector);

    QPushButton *removeThisButton = new QPushButton("Удалить");

    QTableWidgetItem *item = new QTableWidgetItem;
    ui->tableWidget->setItem(new_row, 0, item);

    ui->tableWidget->setCellWidget(new_row, 1, removeThisButton);

    connect(removeThisButton, &QPushButton::clicked, [item,this]{
        int removedRow = item->row();
        /*int removedHeight = this->ui->tableWidget->rowHeight(removedRow);
        if (this->ui->tableWidget->rowCount()<5) {
            this->setMinimumHeight(this->minimumHeight() - removedHeight);
            this->setMaximumHeight(this->maximumHeight() - removedHeight);
            this->ui->tableWidget->setMinimumHeight(this->ui->tableWidget->height() - removedHeight);
            this->ui->tableWidget->setMaximumHeight(this->ui->tableWidget->height() - removedHeight);
        }*/
        this->ui->tableWidget->removeRow(removedRow);
    });

    /*if (ui->tableWidget->rowCount()<5) {
        setMinimumHeight(minimumHeight() + newSelector->height());
        setMaximumHeight(maximumHeight() + newSelector->height());
        ui->tableWidget->setMinimumHeight(ui->tableWidget->height() + newSelector->height());
        ui->tableWidget->setMaximumHeight(ui->tableWidget->height() + newSelector->height());
    }*/
}

void DishAdderWidget::onSendButtonClicked()
{
    jsonSender->addDish(getInfo());
}

QJsonObject DishAdderWidget::getInfo()
{
    QJsonObject res;
    res["title"] = ui->nameLabel->text();
    res["mass"] = ui->massSpinBox->value();
    res["cost"] = ui->priceSpinBox->value();
    res["describe"] = ui->descriptionEdit->toPlainText();
    res["tags"] = QJsonArray::fromStringList(ui->tagsEdit->text().split(","));
    res["photo"] = imageSelector->currentText();
    QJsonArray ingredientsArray;
    for (int i = 0; i < ui->tableWidget->rowCount() - 1; i++) {
        QJsonObject ingredientObj = static_cast<IngredientSelector*>(ui->tableWidget->cellWidget(i,0))->getInfo();
        if (ingredientObj["id"] != 0) {
            ingredientsArray.append(ingredientObj);
        }
    }
    res["ingredients"] = ingredientsArray;
    return res;
}
