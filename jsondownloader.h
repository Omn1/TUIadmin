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
    explicit JsonDownloader(QObject *parent = nullptr);

    QJsonArray getOrders();
    QJsonObject getDishById(int id);
    QJsonObject getIngredientById(int id);
    QPixmap getPixmap(const QString &name);
    void start();
public slots:
    void onGetUpdateInfo(QNetworkReply *reply);
    void onDownloadedOrders(QNetworkReply *reply);
    void onDownloadedDishes(QNetworkReply *reply);
    void onDownloadedIngredients(QNetworkReply *reply);
    void onDownloadedImages(QNetworkReply *reply);
    void onDownloadedPixmap(QNetworkReply *reply);

signals:
    void updateReady();

private:

    void getUpdate();

    void getUpdateInfo();
    void getPixmapFromServer();
    void getImages();
    void getDishes();
    void getIngredientsFromServer();
    void getOrdersFromServer();

    bool started;
    bool was_updated;

    QString updateOrdersHash;
    QString updateDishesHash;
    QString updateIngredientsHash;
    QString updateImgHash;

    QString currentOrdersHash;
    QString currentDishesHash;
    QString currentIngredientsHash;
    QString currentImgHash;

    QJsonArray dishList;
    QJsonArray orderList;
    QJsonArray ingredientList;

    QStringList imageNames;
    int cur_img;
    std::map<QString, QPixmap> imgCache;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
};

#endif // JSONDOWNLOADER_H
