#ifndef CPSWEBSOCKETCLIENT_H
#define CPSWEBSOCKETCLIENT_H

#include <QObject>
#include <QUrl>
#include <QtWebSockets/QWebSocket>
#include <QDebug>
#include <QJsonArray>
#include "cpswebsocketrequest.h"

namespace CPS {

class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    enum class ResponseType {
        Authenticate,
        History,
        RFID
    };

    explicit WebSocketClient(QObject *parent = nullptr);

Q_SIGNALS:
    void newUser(const QString &username, const QString &date, const QString &time);
    void connectionChanged(bool enabled);
    void historyReady(const QJsonArray &history);

public Q_SLOTS:
    void connectToServer(const QString &address, const QString &username, const QString &password);
    void getHistory();

private Q_SLOTS:
    void connected();
    void errorOccurred(QAbstractSocket::SocketError error);
    void findRequest(const QString &data);

private:
    ResponseType findResponseType(const QJsonDocument &jsonDocument);

private:
    QWebSocket _webSocket;
    WebSocketRequest _request;

    QString _username;
    QString _password;
};

}

#endif
