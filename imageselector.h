#ifndef IMAGESELECTOR_H
#define IMAGESELECTOR_H

#include <QWidget>
#include <QComboBox>
#include "jsondownloader.h"

class ImageSelector : public QComboBox
{
    Q_OBJECT
public:
    explicit ImageSelector(QWidget *parent = nullptr, JsonDownloader *jsonLoader = nullptr);

signals:

public slots:
    void onNewImages();

private:
    JsonDownloader *loader;
};

#endif // IMAGESELECTOR_H
