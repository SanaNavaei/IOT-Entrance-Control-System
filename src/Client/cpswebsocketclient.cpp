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
    qDebug() << data;
}

void WebSocketClient::connectToServer(const QString &address, const QString &username, const QString &password)
{
    QUrl url = QUrl(address);
    _webSocket.open(url);

    _username = username;
    _password = password;
}    

} // end of CPS
