#include "jsonsender.h"

QString JsonSender::loginInfo = "";
QString JsonSender::APIurl = "http://xlvzero.tk:5000";

JsonSender::JsonSender(QObject *parent)
    : QObject(parent)
    , manager(new QNetworkAccessManager)
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
    sendJsonTo(QUrl(APIurl+"/add/dish"+"?"+loginInfo), json);
}

void JsonSender::addIngredent(const QJsonObject &json)
{
    sendJsonTo(QUrl(APIurl+"/add/ingredient"+"?"+loginInfo), json);
}

void JsonSender::supplyIngredient(const QJsonObject &json)
{
    sendJsonTo(QUrl(APIurl+"/supply"+"?"+loginInfo), json);
}

void JsonSender::deleteSupply(int supply_id)
{
    makeGetRequest(QUrl(APIurl+"/delete/supply/"+QString::number(supply_id)+"?"+loginInfo));
}

void JsonSender::deleteDish(int dish_id)
{
    makeGetRequest(QUrl(APIurl+"/delete/dish/"+QString::number(dish_id)+"?"+loginInfo));
}

void JsonSender::confirmOrder(int order_id)
{
    makeGetRequest(QUrl(APIurl+"/confirm/order/"+QString::number(order_id)+"?"+loginInfo));
}

void JsonSender::cookOrder(int order_id)
{
    makeGetRequest(QUrl(APIurl+"/cook/order/"+QString::number(order_id)+"?"+loginInfo));
}

void JsonSender::authenticate(const QString &login, const QString &password)
{
    makeGetRequest(QUrl(APIurl+"/login?login="+login+"&password="+password));
}

void JsonSender::onJsonSent(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << reply->errorString() << "\n";
        emit jsonSent(0);
        return;
    }
    QByteArray answer = reply->readAll();
    lastAnswer = QJsonDocument::fromJson(answer).object();
    emit jsonSent(1);
}
