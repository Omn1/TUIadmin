#ifndef INGREDIENTADDERWIDGET_H
#define INGREDIENTADDERWIDGET_H

#include <QWidget>
#include "jsondownloader.h"
#include "jsonsender.h"

namespace Ui {
class IngredientAdderWidget;
}

class IngredientAdderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IngredientAdderWidget(QWidget *parent = nullptr, JsonDownloader *jsonLoader = nullptr);
    ~IngredientAdderWidget();

    QJsonObject getInfo();
public slots:
    void onSendButtonClicked();
private:
    Ui::IngredientAdderWidget *ui;
    JsonDownloader *loader;
    JsonSender *jsonSender;
};

#endif // INGREDIENTADDERWIDGET_H
