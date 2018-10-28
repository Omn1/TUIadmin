#include "jsonsender.h"
#include "sessioninfo.h"

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
    sendJsonTo(QUrl(SessionInfo::APIurl+"/add/dish"+"?"+SessionInfo::loginInfo), json);
}

void JsonSender::addIngredent(const QJsonObject &json)
{
    sendJsonTo(QUrl(SessionInfo::APIurl+"/add/ingredient"+"?"+SessionInfo::loginInfo), json);
}

void JsonSender::supplyIngredient(const QJsonObject &json)
{
    sendJsonTo(QUrl(SessionInfo::APIurl+"/supply"+"?"+SessionInfo::loginInfo), json);
}

void JsonSender::deleteSupply(int supply_id)
{
    makeGetRequest(QUrl(SessionInfo::APIurl+"/delete/supply/"+QString::number(supply_id)+"?"+SessionInfo::loginInfo));
}

void JsonSender::deleteDish(int dish_id)
{
    makeGetRequest(QUrl(SessionInfo::APIurl+"/delete/dish/"+QString::number(dish_id)+"?"+SessionInfo::loginInfo));
}

void JsonSender::deleteIngredient(int ingredient_id)
{
    makeGetRequest(QUrl(SessionInfo::APIurl+"/delete/ingredient/"+QString::number(ingredient_id)+"?"+SessionInfo::loginInfo));
}

void JsonSender::confirmOrder(int order_id)
{
    makeGetRequest(QUrl(SessionInfo::APIurl+"/confirm/order/"+QString::number(order_id)+"?"+SessionInfo::loginInfo));
}

void JsonSender::declineOrder(int order_id)
{
    makeGetRequest(QUrl(SessionInfo::APIurl+"/delete/order/"+QString::number(order_id)+"?"+SessionInfo::loginInfo));
}

void JsonSender::cookOrder(int order_id)
{
    makeGetRequest(QUrl(SessionInfo::APIurl+"/cook/order/"+QString::number(order_id)+"?"+SessionInfo::loginInfo));
}

void JsonSender::authenticate(const QString &login, const QString &password, int cafe_id)
{
    makeGetRequest(QUrl(SessionInfo::APIurl+"/login?login="+login+"&password="+password+"&cafe_id="+QString::number(cafe_id)));
}

void JsonSender::getCafeList()
{
    makeGetRequest(QUrl(SessionInfo::APIurl+"/get/cafes"));
}

void JsonSender::deleteEmployee(int employee_id)
{
    makeGetRequest(QUrl(SessionInfo::APIurl+"/delete/employee/"+QString::number(employee_id)+"?"+SessionInfo::loginInfo));
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
    int status = lastAnswer["status"].toInt();
    emit jsonSent(status != 400 && status != 0);
}
