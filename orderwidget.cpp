#include "orderwidget.h"
#include "dishwidget.h"
#include <QDebug>
/*OrderWidget::OrderWidget(QTreeWidgetItem *parent)
    : QTreeWidgetItem(parent)
    , contents(new QWidget)
    , mainLayout(new QVBoxLayout)
    , toggleButton(new QToolButton)
    , orderInfo(new OrderInfoWidget)
{
    contents->setLayout(mainLayout);
    qDebug() << (parent->treeWidget()) << "\n";
    treeWidget()->setItemWidget(this, 0, contents);

    setupContents();
}*/

OrderWidget::OrderWidget(QTreeWidget *parent)
    : QTreeWidgetItem(parent)
    , contents(new QWidget)
    , mainLayout(new QVBoxLayout)
    , toggleButton(new QToolButton)
    , orderInfo(new OrderInfoWidget)
{
    setupContents();
    treeWidget()->setItemWidget(this, 0, contents);
}

void OrderWidget::setupContents()
{
    contents->setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    contents->setContentsMargins(0,0,0,0);

    toggleButton->setStyleSheet("QToolButton { border: none; }");
    toggleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toggleButton->setArrowType(Qt::ArrowType::RightArrow);
    toggleButton->setText("Блюда");
    toggleButton->setCheckable(true);
    toggleButton->setChecked(false);

    QObject::connect(toggleButton, &QToolButton::clicked, [this](const bool checked) {
        toggleButton->setArrowType(checked ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
        this->setExpanded(checked);
    });

    mainLayout->addWidget(orderInfo);
    treeWidget()->setColumnWidth(0,std::max(orderInfo->width(), treeWidget()->columnWidth(0)));
    mainLayout->addWidget(toggleButton);
}

void OrderWidget::loadFromJSON(const QJsonObject &json, JsonDownloader &loader)
{
    orderInfo->setID(QString::number(json["id"].toInt()));
    orderInfo->setAddress(json["address"].toString());
    orderInfo->setPhone(json["phone"].toString());
    orderInfo->setName(json["name"].toString());
    QJsonArray dishes = json["dishes"].toArray();
    for(int i = 0; i < dishes.size(); i++){
        QJsonObject dishObj = dishes[i].toObject();
        DishWidget *dish = new DishWidget(this);
        dish->setFont(1, QFont("Sans Serif", 16));
        dish->setText(1, "Количество: "+QString::number(dishObj["number"].toInt()));
        //qDebug() << loader.getDishById(dishObj["dish_id"].toInt()) << "\n";
        dish->loadFromJSON(loader.getDishById(dishObj["id"].toInt()), loader);
    }
}

OrderWidget::~OrderWidget()
{
    //delete ui;
}
