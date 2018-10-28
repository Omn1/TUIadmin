#include "ingredientcombobox.h"

IngredientComboBox::IngredientComboBox(QWidget *parent, JsonDownloader *jsonLoader)
    : QComboBox(parent)
    , loader(jsonLoader)
{
    if(!loader){
        loader = new JsonDownloader;
        connect(loader, &JsonDownloader::updateReady, this, &IngredientComboBox::onNewIngredients);
        loader->start();
    }
    else{
        connect(loader, &JsonDownloader::updateReady, this, &IngredientComboBox::onNewIngredients);
    }
    onNewIngredients();
}

void IngredientComboBox::onNewIngredients()
{
    int prev_index = count() ? currentIndex() : 0;
    clear();
    addItem(QString("Выберите ингредиент"), 0);
    QJsonArray ingredients = loader->getIngredients();
    for (int i = 0; i < ingredients.size(); i++) {
        QJsonObject json = ingredients[i].toObject();
        addItem(json["title"].toString(), json["id"].toInt());
    }
    setCurrentIndex(prev_index);
}
