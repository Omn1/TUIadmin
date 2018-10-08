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

    void sendJsonTo(const QUrl &url, const QJsonObject &json);
    void addDish(const QJsonObject &json);
    void addIngredent(const QJsonObject &json);
signals:
    void jsonSent(bool);
public slots:
    void onJsonSent(QNetworkReply *reply);
private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
};

#endif // JSONSENDER_H
