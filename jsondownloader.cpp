#include "jsondownloader.h"
#include "sessioninfo.h"
#include <QDebug>
#include <QSslSocket>

JsonDownloader::JsonDownloader(QObject *parent, bool downloadPhotosFlag)
    : QObject(parent)
    , downloadPhotosFlag(downloadPhotosFlag)
    , started(0)
    , was_updated(0)
    , cur_img(-1)
    , manager(new QNetworkAccessManager)
    , imgManager(new QNetworkAccessManager)
{
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    imgRequest.setSslConfiguration(QSslConfiguration::defaultConfiguration());
}

QJsonArray JsonDownloader::getOrders()
{
    return orderList;
}

QJsonArray JsonDownloader::getWarehouseInfo()
{
    return warehouseList;
}

QJsonArray JsonDownloader::getDishes()
{
    return dishList;
}

QJsonArray JsonDownloader::getIngredients()
{
    return ingredientList;
}

QJsonArray JsonDownloader::getSupplyHistory()
{
    return supplyHistoryList;
}

QJsonArray JsonDownloader::getPersonnelInfo()
{
    return personnelList;
}

QJsonObject JsonDownloader::getDishById(int id)
{
    for (int i = 0; i < dishList.size(); i++)
    {
        if (dishList[i].toObject()["id"].toInt() == id)
            return dishList[i].toObject();
    }
    return QJsonObject();
}

QJsonObject JsonDownloader::getIngredientById(int id)
{
    for (int i = 0; i < ingredientList.size(); i++)
    {
        if (ingredientList[i].toObject()["id"].toInt() == id)
            return ingredientList[i].toObject();
    }
    return QJsonObject();
}

QPixmap JsonDownloader::getPixmap(const QString &name)
{
    QPixmap res = imgCache[name];
    return res;
}

QStringList JsonDownloader::getImageNames()
{
    return imageNames;
}

void JsonDownloader::start()
{
    if (!started)
    {
        started = 1;
        getUpdateInfo();
    }
}

void JsonDownloader::onGetUpdateInfo(QNetworkReply *reply)
{
    disconnect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onGetUpdateInfo(QNetworkReply*)));

    if(reply->error())
    {
        qDebug() << reply->errorString() << "\n";
        getUpdateInfo();
        return;
    }
    QByteArray answer = reply->readAll();
    //qDebug() << answer << "\n";
    QJsonObject response = QJsonDocument::fromJson(answer).object();
    //qDebug() << response << "\n";

    QJsonObject json = response["res"].toObject();

    updateImgHash = json["pictures_hash"].toString();
    updateDishesHash = json["dishes_hash"].toString();
    updateOrdersHash = json["orders_hash"].toString();
    updateIngredientsHash = json["ingredient_hash"].toString();
    updateWarehouseHash = json["supplies_hash"].toString();
    updateSupplyHistoryHash = json["supply_logs_hash"].toString();
    updatePersonnelHash = json["employees_hash"].toString();

    getUpdate();
}

void JsonDownloader::onDownloadedOrders(QNetworkReply *reply)
{
    disconnect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadedOrders(QNetworkReply*)));

    if(reply->error())
    {
        qDebug() << reply->errorString() << "\n";
        getOrdersFromServer();
        return;
    }

    QByteArray answer = reply->readAll();

    QJsonObject response = QJsonDocument::fromJson(answer).object();

    orderList = response["res"].toArray();

    getUpdate();
}

void JsonDownloader::onDownloadedDishes(QNetworkReply *reply)
{
    disconnect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadedDishes(QNetworkReply*)));

    if(reply->error())
    {
        qDebug() << reply->errorString() << "\n";
        getDishesFromServer();
        return;
    }

    QByteArray answer = reply->readAll();

    QJsonObject response = QJsonDocument::fromJson(answer).object();

    dishList = response["res"].toArray();

    getUpdate();
}

void JsonDownloader::onDownloadedIngredients(QNetworkReply *reply)
{
    disconnect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadedIngredients(QNetworkReply*)));

    if(reply->error())
    {
        qDebug() << reply->errorString() << "\n";
        getIngredientsFromServer();
        return;
    }

    QByteArray answer = reply->readAll();

    QJsonObject response = QJsonDocument::fromJson(answer).object();

    ingredientList = response["res"].toArray();

    getUpdate();
}

void JsonDownloader::onDownloadedImages(QNetworkReply *reply)
{
    disconnect(imgManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadedImages(QNetworkReply*)));

    if(reply->error())
    {
        qDebug() << reply->errorString() << "\n";
        getImages();
        return;
    }

    QByteArray answer = reply->readAll();

    QJsonObject response = QJsonDocument::fromJson(answer).object();

    QJsonArray jsonNames = response["res"].toArray();

    qDebug() << jsonNames << "\n";
    imageNames.clear();

    for (int i = 0; i < jsonNames.size(); i++)
    {
        imageNames.append(jsonNames[i].toString());
    }

    cur_img = imageNames.size() - 1;

    imgCache.clear();

    getPixmapFromServer();
}

void JsonDownloader::onDownloadedPixmap(QNetworkReply *reply)
{
    disconnect(imgManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadedPixmap(QNetworkReply*)));

    if(reply->error())
    {
        qDebug() << reply->errorString() << "\n";
        getPixmapFromServer();
        return;
    }

    QByteArray answer = reply->readAll();

    QPixmap pixmap = QPixmap::fromImage(QImage::fromData(answer));

    imgCache[imageNames[cur_img]] = pixmap;

    cur_img--;
    getPixmapFromServer();
}

void JsonDownloader::onDownloadedWarehouseInfo(QNetworkReply *reply)
{
    disconnect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadedWarehouseInfo(QNetworkReply*)));

    if(reply->error())
    {
        qDebug() << reply->errorString() << "\n";
        getWarehouseInfoFromServer();
        return;
    }

    QByteArray answer = reply->readAll();

    QJsonObject response = QJsonDocument::fromJson(answer).object();

    warehouseList = response["res"].toArray();

    getUpdate();
}

void JsonDownloader::onDownloadedSupplyHistory(QNetworkReply *reply)
{
    disconnect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadedSupplyHistory(QNetworkReply*)));

    if(reply->error())
    {
        qDebug() << reply->errorString() << "\n";
        getSupplyHistoryFromServer();
        return;
    }

    QByteArray answer = reply->readAll();

    QJsonObject response = QJsonDocument::fromJson(answer).object();

    supplyHistoryList = response["res"].toArray();

    getUpdate();
}

void JsonDownloader::onDownloadedPersonnelInfo(QNetworkReply *reply)
{
    disconnect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadedPersonnelInfo(QNetworkReply*)));

    if(reply->error())
    {
        qDebug() << reply->errorString() << "\n";
        getPersonnelInfoFromServer();
        return;
    }

    QByteArray answer = reply->readAll();

    QJsonObject response = QJsonDocument::fromJson(answer).object();

    personnelList = response["res"].toArray();

    getUpdate();
}

void JsonDownloader::getUpdate()
{
    if (downloadPhotosFlag && updateImgHash != currentImgHash)
    {
        getImages();
    }
    if (updateIngredientsHash != currentIngredientsHash)
    {
        getIngredientsFromServer();
    }
    else if (updateDishesHash != currentDishesHash)
    {
        getDishesFromServer();
    }
    else if (updateOrdersHash != currentOrdersHash)
    {
        getOrdersFromServer();
    }
    else if(updateWarehouseHash != currentWarehouseHash){
        getWarehouseInfoFromServer();
    }
    else if(updateSupplyHistoryHash != currentSupplyHistoryHash){
        getSupplyHistoryFromServer();
    }
    else if(updatePersonnelHash != currentPersonnelHash){
        getPersonnelInfoFromServer();
    }
    else{
        if(was_updated)
            emit updateReady();
        getUpdateInfo();
    }
}

void JsonDownloader::getUpdateInfo()
{
    //qDebug () << "getUpdateInfo\n";
    currentImgHash = updateImgHash;
    currentDishesHash = updateDishesHash;
    currentOrdersHash = updateOrdersHash;
    was_updated = 0;
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onGetUpdateInfo(QNetworkReply*)));
    request.setUrl(QUrl(SessionInfo::APIurl+"/get/state_hashes"+"?"+SessionInfo::loginInfo));
    manager->get(request);
}

void JsonDownloader::getPixmapFromServer()
{
    qDebug () << "getPixmap\n";
    if (cur_img == -1)
    {
        emit updateReady();
        return;
    }
    QString pixmapName = imageNames[cur_img];
    connect(imgManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadedPixmap(QNetworkReply*)));
    imgRequest.setUrl(QUrl(SessionInfo::APIurl+"/public/" + pixmapName+"?"+SessionInfo::loginInfo));
    imgManager->get(imgRequest);
}

void JsonDownloader::getImages()
{
    qDebug () << "getImages\n";
    currentImgHash = updateImgHash;
    was_updated = 1;
    connect(imgManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadedImages(QNetworkReply*)));
    imgRequest.setUrl(QUrl(SessionInfo::APIurl+"/get/images"+"?"+SessionInfo::loginInfo));
    imgManager->get(imgRequest);
}

void JsonDownloader::getDishesFromServer()
{
    qDebug () << "getDishes\n";
    currentDishesHash = updateDishesHash;
    was_updated = 1;
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadedDishes(QNetworkReply*)));
    request.setUrl(QUrl(SessionInfo::APIurl+"/get/dishes"+"?"+SessionInfo::loginInfo));
    manager->get(request);
}

void JsonDownloader::getIngredientsFromServer()
{
    qDebug () << "getIngredients\n";
    currentIngredientsHash = updateIngredientsHash;
    was_updated = 1;
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadedIngredients(QNetworkReply*)));
    request.setUrl(QUrl(SessionInfo::APIurl+"/get/ingredients"+"?"+SessionInfo::loginInfo));
    manager->get(request);
}

void JsonDownloader::getOrdersFromServer()
{
    qDebug () << "getOrders\n";
    currentOrdersHash = updateOrdersHash;
    was_updated = 1;
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadedOrders(QNetworkReply*)));
    request.setUrl(QUrl(SessionInfo::APIurl+"/get/orders"+"?"+SessionInfo::loginInfo));
    manager->get(request);
}

void JsonDownloader::getWarehouseInfoFromServer()
{
    qDebug() << "getWarehouse\n";
    currentWarehouseHash = updateWarehouseHash;
    was_updated = 1;
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadedWarehouseInfo(QNetworkReply*)));
    request.setUrl(QUrl(SessionInfo::APIurl+"/get/goods"+"?"+SessionInfo::loginInfo));
    manager->get(request);
}

void JsonDownloader::getSupplyHistoryFromServer()
{
    qDebug() << "getSupplyHistory\n";
    currentSupplyHistoryHash = updateSupplyHistoryHash;
    was_updated = 1;
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadedSupplyHistory(QNetworkReply*)));
    request.setUrl(QUrl(SessionInfo::APIurl+"/get/supply_history"+"?"+SessionInfo::loginInfo));
    manager->get(request);
}

void JsonDownloader::getPersonnelInfoFromServer()
{
    qDebug() << "getPersonnelInfo\n";
    currentPersonnelHash = updatePersonnelHash;
    was_updated = 1;
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadedPersonnelInfo(QNetworkReply*)));
    request.setUrl(QUrl(SessionInfo::APIurl+"/get/employees"+"?"+SessionInfo::loginInfo));
    manager->get(request);
}
