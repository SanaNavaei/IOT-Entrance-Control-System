#ifndef CPSWEBSOCKETCLIENT_H
#define CPSWEBSOCKETCLIENT_H

#include <QObject>
#include <QUrl>
#include <QtWebSockets/QWebSocket>
#include <QDebug>
#include "cpswebsocketrequest.h"

namespace CPS {

class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClient(QObject *parent = nullptr);

Q_SIGNALS:
    void newUser();
    void connectionChanged();

public Q_SLOTS:
    void connectToServer(const QString &address, const QString &username, const QString &password);

private Q_SLOTS:
    void connected();
    void findRequest(const QString &data);

private:
    QWebSocket _webSocket;
    WebSocketRequest _request;

    QString _username;
    QString _password;
};

}

#endif
