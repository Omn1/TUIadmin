#include "ingredientselector.h"
#include "ui_ingredientselector.h"
IngredientSelector::IngredientSelector(QWidget *parent, JsonDownloader *jsonLoader)
    : QWidget(parent)
    , ui(new Ui::IngredientSelector)
    , loader(jsonLoader)
{
    ui->setupUi(this);

    if (jsonLoader == nullptr) {
        loader = new JsonDownloader;
        loader->start();
    }

    //connect(ui->lineEdit, &QLineEdit::textEdited, this, &IngredientSelector::onIdEntered);
    connect(ui->idSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &IngredientSelector::onIdEntered);
}

IngredientSelector::~IngredientSelector()
{
    delete ui;
}

QJsonObject IngredientSelector::getInfo()
{
    QJsonObject res;
    res["mass"] = ui->massSpinBox->value();
    res["id"] = ui->idSpinBox->value();
    res["title"] = loader->getIngredientById(ui->idSpinBox->value())["title"];
    return res;
}

void IngredientSelector::onIdEntered()
{
    //qDebug () << ui->spinBox->value() << "\n";
    ui->ingredientWidget->loadFromJSON(loader->getIngredientById(ui->idSpinBox->value()), *loader);
}
