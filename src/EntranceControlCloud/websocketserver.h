#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QJsonValue>
#include <QWebSocket>
#include <QDateTime>

#include "entrancerecord.h"

class WebSocketServer : public QObject
{
    Q_OBJECT

public:
    WebSocketServer(const QHostAddress &address, quint16 port, QObject *parent = nullptr);

Q_SIGNALS:
    void historyRequested();
    void authenticateRequested(const QString &username, const QString &password);
    void authenticateRFID(const QJsonDocument &jsonDoc);

public slots:
    void sendHistory(const QJsonDocument &jsonDoc);
    void sendAuthenticate();
    void sendUnauthenticate();
    void sendAunthenticatedRFID(const QJsonDocument &jsonDoc);
    void sendUnauthenticatedRFID();

private slots:
    void onNewConnection();
    void processTextMessage(const QString &message);
    void socketDisconnected();

private:
    QHostAddress m_address;
    quint16 m_port;
    QWebSocketServer *m_server;
    QWebSocket *m_client_socket;
    bool m_isAuthenticated = false;
};

#endif // WEBSOCKETSERVER_H
