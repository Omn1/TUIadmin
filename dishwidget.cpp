#include "dishwidget.h"
#include "ingredientwidget.h"
#include <QDebug>

DishWidget::DishWidget(QTreeWidgetItem *parent)
    : QTreeWidgetItem(parent)
    , contents(new QWidget)
    , mainLayout(new QVBoxLayout)
    , toggleButton(new QToolButton)
    , dishInfo(new DishInfoWidget)
{
    setupContents();
    treeWidget()->setItemWidget(this, 0, contents);
}

DishWidget::DishWidget(QTreeWidget *parent)
    : QTreeWidgetItem(parent)
    , contents(new QWidget)
    , mainLayout(new QVBoxLayout)
    , toggleButton(new QToolButton)
    , dishInfo(new DishInfoWidget)
{
    setupContents();
    treeWidget()->setItemWidget(this, 0, contents);
}

DishWidget::~DishWidget()
{
    //delete ui;
}

void DishWidget::setupContents()
{
    contents->setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    contents->setContentsMargins(0,0,0,0);

    toggleButton->setStyleSheet("QToolButton { border: none; }");
    toggleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toggleButton->setArrowType(Qt::ArrowType::RightArrow);
    toggleButton->setText("Ингредиенты");
    toggleButton->setCheckable(true);
    toggleButton->setChecked(false);

    QObject::connect(toggleButton, &QToolButton::clicked, [this](const bool checked) {
        toggleButton->setArrowType(checked ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
        this->setExpanded(checked);
    });

    mainLayout->addWidget(dishInfo);
    treeWidget()->setColumnWidth(0,std::max(dishInfo->width(), treeWidget()->columnWidth(0)));
    mainLayout->addWidget(toggleButton);
}

void DishWidget::loadFromJSON(const QJsonObject &json, JsonDownloader &loader)
{
    dishInfo->setTitle(json["title"].toString());
    dishInfo->setMass(QString::number(json["mass"].toDouble()));
    dishInfo->setPrice(QString::number(json["cost"].toDouble()));
    dishInfo->setImage(loader.getPixmap(json["photo"].toString()));
    QJsonArray ingredients = json["ingredients"].toArray();
    for (int i = 0; i < ingredients.size(); i++){
        QJsonObject obj = ingredients[i].toObject();
        QTreeWidgetItem *ingredientItem = new QTreeWidgetItem(this);
        IngredientWidget *ingredient = new IngredientWidget;
        ingredient->loadFromJSON(obj, loader);
        //ingredient->setMass(QString::number(obj["mass"].toDouble()));
        treeWidget()->setItemWidget(ingredientItem,0,ingredient);
        ingredientItem->setFont(1,QFont("Sans serif",16));
        ingredientItem->setText(1,"Вес: "+QString::number(obj["mass"].toDouble())+"g");
    }
}

