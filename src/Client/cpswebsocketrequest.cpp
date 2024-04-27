#include "cpswebsocketrequest.h"

namespace CPS {

WebSocketRequest::WebSocketRequest() {}

QString WebSocketRequest::authenticationRequest(const QString &username, const QString &password)
{
    QJsonObject jsonObject;
    jsonObject["type"] = "authenticate";

    QJsonObject dataObject;
    dataObject["username"] = username;
    dataObject["password"] = password;

    jsonObject["data"] = dataObject;

    QJsonDocument jsonDocument(jsonObject);
    QString jsonString = jsonDocument.toJson(QJsonDocument::Compact);

    return jsonString;
}

QString WebSocketRequest::historyRequest()
{
    QJsonObject jsonObject;
    jsonObject["type"] = "history";

    QJsonObject dataObject;
    jsonObject["data"] = dataObject;

    QJsonDocument jsonDocument(jsonObject);
    QString jsonString = jsonDocument.toJson(QJsonDocument::Compact);

    return jsonString;
}

} // end of CPS
