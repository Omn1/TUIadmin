#include "jsonsender.h"

JsonSender::JsonSender(QObject *parent)
    : QObject(parent)
    , manager(new QNetworkAccessManager)
{
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    connect(manager, &QNetworkAccessManager::finished, this, &JsonSender::onJsonSent);
}

void JsonSender::sendJsonTo(const QUrl &url, const QJsonObject &json)
{
    request.setUrl(url);

    QJsonDocument jsonDoc(json);
    QByteArray data = jsonDoc.toJson();

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.size()));

    manager->post(request, data);
}

void JsonSender::addDish(const QJsonObject &json)
{
    sendJsonTo(QUrl("http://api.torianik.online:5000/add/dish"), json);
}

void JsonSender::addIngredent(const QJsonObject &json)
{
    sendJsonTo(QUrl("http://api.torianik.online:5000/add/ingredient"), json);
}

void JsonSender::supplyIngredient(const QJsonObject &json)
{
    sendJsonTo(QUrl("http://api.torianik.online:5000/supply"), json);
}

void JsonSender::onJsonSent(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << reply->errorString() << "\n";
        emit jsonSent(0);
        return;
    }
    emit jsonSent(1);
}
