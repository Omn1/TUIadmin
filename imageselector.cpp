#include "imageselector.h"

ImageSelector::ImageSelector(QWidget *parent, JsonDownloader *jsonLoader)
    : QComboBox(parent)
    , loader(jsonLoader)
{
    if(!loader){
        loader = new JsonDownloader;
        connect(loader, &JsonDownloader::updateReady, this, &ImageSelector::onNewImages);
        loader->start();
    }
    else{
        connect(loader, &JsonDownloader::updateReady, this, &ImageSelector::onNewImages);
    }
    onNewImages();
}

void ImageSelector::onNewImages()
{
    clear();
    QStringList imageNames = loader->getImageNames();
    for (int i = 0; i < imageNames.size(); i++) {
        addItem(QIcon(loader->getPixmap(imageNames[i])),imageNames[i]);
    }
}
