#ifndef JSONSENDER_H
#define JSONSENDER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslSocket>
#include <QJsonObject>
#include <QJsonDocument>

class JsonSender : public QObject
{
    Q_OBJECT
public:
    explicit JsonSender(QObject *parent = nullptr);

    void makeGetRequest(const QUrl &url);
    void sendJsonTo(const QUrl &url, const QJsonObject &json);
    void addDish(const QJsonObject &json);
    void addIngredent(const QJsonObject &json);
    void supplyIngredient(const QJsonObject &json);
    void deleteSupply(int supply_id);
signals:
    void jsonSent(bool);
public slots:
    void onJsonSent(QNetworkReply *reply);
private:
    QNetworkAccessManager *manager;
    QNetworkRequest request, simple_request;
};

#endif // JSONSENDER_H
