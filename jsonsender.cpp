#include "jsonsender.h"

JsonSender::JsonSender(QObject *parent, QString APIurl)
    : QObject(parent)
    , manager(new QNetworkAccessManager)
    , APIurl(APIurl)
{
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    simple_request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    connect(manager, &QNetworkAccessManager::finished, this, &JsonSender::onJsonSent);
}

void JsonSender::makeGetRequest(const QUrl &url)
{
    simple_request.setUrl(url);
    manager->get(simple_request);
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
    sendJsonTo(QUrl(APIurl+"/add/dish"), json);
}

void JsonSender::addIngredent(const QJsonObject &json)
{
    sendJsonTo(QUrl(APIurl+"/add/ingredient"), json);
}

void JsonSender::supplyIngredient(const QJsonObject &json)
{
    sendJsonTo(QUrl(APIurl+"/supply"), json);
}

void JsonSender::deleteSupply(int supply_id)
{
    makeGetRequest(QUrl(APIurl+"/delete/supply/"+QString::number(supply_id)));
}

void JsonSender::deleteDish(int dish_id)
{
    makeGetRequest(QUrl(APIurl+"/delete/dish/"+QString::number(dish_id)));
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
