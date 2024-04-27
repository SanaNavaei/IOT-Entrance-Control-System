#include "cpswebsocketclient.h"

namespace CPS {

WebSocketClient::WebSocketClient(QObject *parent)
    : QObject{parent},
      _request()
{
    QObject::connect(&_webSocket, &QWebSocket::connected, this, &WebSocketClient::connected);
}

void WebSocketClient::connected()
{
    QObject::connect(&_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::findRequest);
    qDebug() << "Connected to server.";

    QString authenticationRequest = _request.authenticationRequest(_username, _password);
    _webSocket.sendTextMessage(authenticationRequest);
    qDebug() << "Sent authentication request.";
}

void WebSocketClient::findRequest(const QString &data)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toUtf8());
    ResponseType responseType = findResponseType(jsonDocument);

    switch (responseType)
    {
    case ResponseType::History: {
        QJsonArray jsonArray = jsonDocument.array();
        emit historyReady(jsonArray);
        break; }
    case ResponseType::Authenticate: {
        QJsonObject jsonObject = jsonDocument.object();
        QString isAuthenticated = jsonObject["isAuthenticated"].toString();
        if (isAuthenticated == "false") {
            emit connectionChanged(true);
        }
        break; }
    case ResponseType::RFID: {
        QJsonObject jsonObject = jsonDocument.object();
        qDebug() << jsonObject;
        if (jsonObject["permitted"].toBool())
        {
            QString username = jsonObject["username"].toString();
            QString date = jsonObject["date"].toString();
            QString time = jsonObject["time"].toString();

            emit newUser(username, date, time);
            qDebug() << "New user: " << username;
        }
        break; }
    default:
        break;
    }
}

WebSocketClient::ResponseType WebSocketClient::findResponseType(const QJsonDocument &jsonDocument) {
    if (jsonDocument.isArray()) {
        return ResponseType::History;
    }
    else if (jsonDocument.object().contains("isAuthenticated")) {
        return ResponseType::Authenticate;
    }
    else {
        return ResponseType::RFID;
    }
}

void WebSocketClient::connectToServer(const QString &address, const QString &username, const QString &password)
{
    if (username.isEmpty() || password.isEmpty()) {
        qDebug() << "Username or password is empty.";
        return;
    }

    QUrl url = QUrl(address);
    emit connectionChanged(false);
    _webSocket.open(url);

    _username = username;
    _password = password;
}    

void WebSocketClient::getHistory()
{
    _webSocket.sendTextMessage(_request.historyRequest());
}

} // end of CPS
