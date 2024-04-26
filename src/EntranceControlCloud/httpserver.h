#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QHttpServer>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "authenticator.h"
#include "entrancerecord.h"

class HttpServer : public QObject
{
    Q_OBJECT

public:
    HttpServer(Authenticator* authenticator, const QHostAddress &address = QHostAddress::LocalHost, quint16 port = 5050, QObject *parent = nullptr);
    ~HttpServer();
    void startServer();
    void stopServer();
    QHttpServerResponse handleRequest(const QHttpServerRequest &req);

Q_SIGNALS:
    void serverStarted(const QHostAddress &address, quint16 port);
    void serverStopped();
    void requestReceived(const QHttpServerRequest *req, QHttpServerResponse *resp);
    void newEntraceRecord(EntranceRecord *record);

public slots:
    // void sendResponse(const QHttpServerRequest *req, QHttpServerResponse *resp);

private slots:

private:
    QHostAddress m_address;
    quint16 m_port;
    QHttpServer *m_server;
    Authenticator *m_authenticator;
};

#endif // HTTPSERVER_H
