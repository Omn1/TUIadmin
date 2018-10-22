#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , loader(new JsonDownloader)
{
    openAuthenticationWidget();
}

void MainWindow::openAuthenticationWidget()
{
    QWidget *authenticationWidget = new AuthenticationWidget;
    connect(authenticationWidget, SIGNAL(authenticationPassed()), this, SLOT(openMainMenu()));
    setCentralWidget(authenticationWidget);
}

void MainWindow::openWarehouseTable()
{

    QWidget *warehouseTablePage = makeWrappedWidget(new WarehouseTable(nullptr, loader));
    setCentralWidget(warehouseTablePage);
}

void MainWindow::openSupplyWidget()
{
    QWidget *supplyWidgetPage = makeWrappedWidget(new IngredientSupplierWidget(nullptr, loader));
    setCentralWidget(supplyWidgetPage);
}

void MainWindow::openDishAdder()
{
    QWidget *dishAdderPage = makeWrappedWidget(new DishAdderWidget(nullptr, loader));
    setCentralWidget(dishAdderPage);
}

void MainWindow::openIngredientAdder()
{
    QWidget *ingredientAdderPage = makeWrappedWidget(new IngredientAdderWidget(nullptr, loader));
    setCentralWidget(ingredientAdderPage);
}

void MainWindow::openOrderTable()
{
    QWidget *orderTablePage = makeWrappedWidget(new OrderTable(nullptr, loader));
    setCentralWidget(orderTablePage);
}

void MainWindow::openDishTable()
{
    QWidget *dishTablePage = makeWrappedWidget(new DishTable(nullptr, loader));
    setCentralWidget(dishTablePage);
}

void MainWindow::openMainMenu()
{
    loader->start();
    MainMenuWidget *mainMenu = new MainMenuWidget;
    connect(mainMenu->checkOrdersButton, &QPushButton::clicked, this, &MainWindow::openOrderTable);
    connect(mainMenu->editMenuButton, &QPushButton::clicked, this, &MainWindow::openEditMenu);
    connect(mainMenu->checkWarehouseButton, &QPushButton::clicked, this, &MainWindow::openWarehouseTable);
    connect(mainMenu->supplyButton, &QPushButton::clicked, this, &MainWindow::openSupplyWidget);
    setCentralWidget(mainMenu);
}

void MainWindow::openEditMenu()
{
    EditMenuWidget *editMenu = new EditMenuWidget;
    QWidget *wrappedEditMenu = makeWrappedWidget(editMenu);
    connect(editMenu->addDishButton, &QPushButton::clicked, this, &MainWindow::openDishAdder);
    connect(editMenu->addIngredientButton, &QPushButton::clicked, this, &MainWindow::openIngredientAdder);
    connect(editMenu->checkDishesButton, &QPushButton::clicked, this, &MainWindow::openDishTable);
    setCentralWidget(wrappedEditMenu);
}

QWidget * MainWindow::makeWrappedWidget(QWidget *widget)
{
    QWidget *wrappedWidget = new QWidget;
    QVBoxLayout *vlayout = new QVBoxLayout;
    QPushButton *returnButton = new QPushButton("Вернуться");
    returnButton->setMaximumWidth(100);
    connect(returnButton, &QPushButton::clicked, this, &MainWindow::openMainMenu);
    vlayout->addWidget(returnButton);
    vlayout->addWidget(widget);
    wrappedWidget->setLayout(vlayout);
    return wrappedWidget;
}
