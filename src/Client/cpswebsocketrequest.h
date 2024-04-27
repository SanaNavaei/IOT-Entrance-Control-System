#ifndef CPSWEBSOCKETREQUEST_H
#define CPSWEBSOCKETREQUEST_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

namespace CPS {

class WebSocketRequest
{
public:
    WebSocketRequest();
    QString authenticationRequest(const QString &username, const QString &password);
    QString historyRequest();
};

}

#endif
