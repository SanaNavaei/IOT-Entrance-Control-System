#include "cpswebsocketclient.h"

namespace CPS {

WebSocketClient::WebSocketClient(QObject *parent)
    : QObject{parent}
{
    QObject::connect(&_webSocket, &QWebSocket::connected, this, &WebSocketClient::connected);
}

void WebSocketClient::connected()
{
    QObject::connect(&_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::findRequest);
    qDebug() << "Connected to server.";
}

void WebSocketClient::findRequest(const QString &data)
{
    qDebug() << data;
}

void WebSocketClient::connectToServer(const QString &address)
{
    QUrl url = QUrl(address);
    _webSocket.open(url);
}    

} // end of CPS
