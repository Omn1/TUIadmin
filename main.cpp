#include "orderwidget.h"
#include "dishwidget.h"
#include "jsondownloader.h"
#include "ordertable.h"
#include "warehousetable.h"
#include "ingredientselector.h"
#include "dishadderwidget.h"
#include <QApplication>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QTreeWidget>
#include <QHeaderView>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //JsonDownloader downloader;

   /* QVBoxLayout *vLayout = new QVBoxLayout;
    QTreeWidget *treeWidget = new QTreeWidget;

    treeWidget->header()->hide();
    treeWidget->setAnimated(true);
    treeWidget->setColumnCount(2);
    treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    treeWidget->setFocusPolicy(Qt::NoFocus);
    treeWidget->setRootIsDecorated(false);
    treeWidget->setIndentation(0);
    {
        OrderWidget *widget = new OrderWidget(treeWidget);
        DishWidget *dish = new DishWidget(widget);
        //widget->loadFromJSON();
    }

    vLayout->addWidget(treeWidget);
*/
    JsonDownloader *loader = new JsonDownloader;

    OrderTable *table = new OrderTable(nullptr,loader);
    table->show();

    //IngredientSelector *selector = new IngredientSelector(nullptr,loader);
    //selector->show();

    DishAdderWidget *dishAdder = new DishAdderWidget(nullptr, loader);
    dishAdder->show();


    loader->start();
    //table->setDisplayedOrder(23);
    /*//vLayout->addWidget(table);

    QWidget *ScrollAreaContent = new QWidget;
    ScrollAreaContent->setLayout(vLayout);
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setStyleSheet("border: 1px solid red");
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(ScrollAreaContent);
    scrollArea->show();*/
    return a.exec();
}
