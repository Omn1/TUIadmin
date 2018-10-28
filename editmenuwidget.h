#ifndef EDITMENUWIDGET_H
#define EDITMENUWIDGET_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class EditMenuWidget;
}

class EditMenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditMenuWidget(QWidget *parent = nullptr);
    ~EditMenuWidget();

    QPushButton *addDishButton;
    QPushButton *addIngredientButton;
    QPushButton *checkDishesButton;
    QPushButton *checkIngredientsButton;
    QPushButton *uploadPhotoButton;
private:
    Ui::EditMenuWidget *ui;
};

#endif // EDITMENUWIDGET_H
