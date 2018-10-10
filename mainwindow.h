#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "mainmenuwidget.h"
#include "editmenuwidget.h"
#include "jsondownloader.h"
#include "ordertable.h"
#include "warehousetable.h"
#include "ingredientselector.h"
#include "dishadderwidget.h"
#include "ingredientadderwidget.h"
#include "ingredientsupplierwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

public slots:
    void openMainMenu();
    void openEditMenu();
    void openOrderTable();
    void openWarehouseTable();
    void openSupplyWidget();
    void openDishAdder();
    void openIngredientAdder();
private:
    QWidget *makeWrappedWidget(QWidget *widget);

    JsonDownloader *loader;
};

#endif // MAINWINDOW_H
