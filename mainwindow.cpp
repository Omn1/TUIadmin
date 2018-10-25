#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , loader(new JsonDownloader)
{
    statEngine = new StatisticsEngine(nullptr,loader);
    openAuthenticationWidget();
}

void MainWindow::openAuthenticationWidget()
{
    QWidget *authenticationWidget = new AuthenticationWidget;
    connect(authenticationWidget, SIGNAL(authenticationPassed()), this, SLOT(openMainMenu()));
    setCentralWidget(authenticationWidget);
}

void MainWindow::openWarehouseTable(int displayed_ingredient_id)
{
    WarehouseTable *warehouseTable = new WarehouseTable(nullptr, loader);
    warehouseTable->setDisplayedIngredientId(displayed_ingredient_id);
    QWidget *warehouseTablePage = makeWrappedWidget(warehouseTable);
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

void MainWindow::openIngredientTable()
{
    IngredientTable *ingredientTable = new IngredientTable(nullptr, loader);
    QWidget *ingredientTablePage = makeWrappedWidget(ingredientTable);
    connect(ingredientTable, &IngredientTable::checkWarehouse, this, &MainWindow::openWarehouseTable);
    setCentralWidget(ingredientTablePage);
}

void MainWindow::openIncomeStats()
{
    QChartView *cv = new QChartView(statEngine->getRecentIncomeChart(7));
    cv->setRenderHint(QPainter::Antialiasing);
    cv->setMinimumSize(800,600);
    setCentralWidget(makeWrappedWidget(cv));
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
    connect(mainMenu->checkWarehouseButton, SIGNAL(clicked()), this, SLOT(openWarehouseTable()));
    connect(mainMenu->supplyButton, &QPushButton::clicked, this, &MainWindow::openSupplyWidget);
    connect(mainMenu->statsButton, &QPushButton::clicked, this, &MainWindow::openIncomeStats);
    setCentralWidget(mainMenu);
}

void MainWindow::openEditMenu()
{
    EditMenuWidget *editMenu = new EditMenuWidget;
    QWidget *wrappedEditMenu = makeWrappedWidget(editMenu);
    connect(editMenu->addDishButton, &QPushButton::clicked, this, &MainWindow::openDishAdder);
    connect(editMenu->addIngredientButton, &QPushButton::clicked, this, &MainWindow::openIngredientAdder);
    connect(editMenu->checkDishesButton, &QPushButton::clicked, this, &MainWindow::openDishTable);
    connect(editMenu->checkIngredientsButton, &QPushButton::clicked, this, &MainWindow::openIngredientTable);
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
