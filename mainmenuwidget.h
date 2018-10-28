#ifndef MAINMENUWIDGET_H
#define MAINMENUWIDGET_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class MainMenuWidget;
}

class MainMenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenuWidget(QWidget *parent = nullptr);
    ~MainMenuWidget();

    QPushButton *checkOrdersButton;
    QPushButton *editMenuButton;
    QPushButton *checkWarehouseButton;
    QPushButton *supplyButton;
    QPushButton *supplyHistoryButton;
    QPushButton *statsButton;
    QPushButton *personnelButton;
public slots:

private:
    Ui::MainMenuWidget *ui;
};

#endif // MAINMENUWIDGET_H
