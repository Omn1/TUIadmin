#ifndef JSONDOWNLOADER_H
#define JSONDOWNLOADER_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslSocket>
#include <QLineEdit>
#include <QStackedWidget>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>
#include <QPixmapCache>
#include <QStringList>
#include <map>

class JsonDownloader : public QObject
{
    Q_OBJECT
public:
    explicit JsonDownloader(QObject *parent = nullptr, bool downloadPhotosFlag = true);

    QJsonArray getOrders();
    QJsonArray getWarehouseInfo();
    QJsonArray getDishes();
    QJsonArray getIngredients();
    QJsonObject getDishById(int id);
    QJsonObject getIngredientById(int id);
    QPixmap getPixmap(const QString &name);
    QStringList getImageNames();
    void start();

    bool downloadPhotosFlag;
    static QString loginInfo;
    static QString APIurl;
public slots:
    void onGetUpdateInfo(QNetworkReply *reply);
    void onDownloadedOrders(QNetworkReply *reply);
    void onDownloadedDishes(QNetworkReply *reply);
    void onDownloadedIngredients(QNetworkReply *reply);
    void onDownloadedImages(QNetworkReply *reply);
    void onDownloadedPixmap(QNetworkReply *reply);
    void onDownloadedWarehouseInfo(QNetworkReply *reply);

signals:
    void updateReady();

private:

    void getUpdate();

    void getUpdateInfo();
    void getPixmapFromServer();
    void getImages();
    void getDishesFromServer();
    void getIngredientsFromServer();
    void getOrdersFromServer();
    void getWarehouseInfoFromServer();

    bool started;
    bool was_updated;

    QString updateOrdersHash;
    QString updateDishesHash;
    QString updateIngredientsHash;
    QString updateWarehouseHash;
    QString updateImgHash;

    QString currentOrdersHash;
    QString currentDishesHash;
    QString currentIngredientsHash;
    QString currentWarehouseHash;
    QString currentImgHash;

    QJsonArray dishList;
    QJsonArray orderList;
    QJsonArray ingredientList;
    QJsonArray warehouseList;

    QStringList imageNames;
    int cur_img;
    std::map<QString, QPixmap> imgCache;
    QNetworkAccessManager *manager, *imgManager;
    QNetworkRequest request, imgRequest;
};

#endif // JSONDOWNLOADER_H
