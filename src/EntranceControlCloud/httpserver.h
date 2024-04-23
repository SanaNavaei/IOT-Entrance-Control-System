#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QHttpServer>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class HttpServer : public QObject
{
    Q_OBJECT

public:
    explicit HttpServer(int port = 8080, QObject *parent = nullptr);
    ~HttpServer();
    void startServer();
    void stopServer();
    QHttpServerResponse handleRequest(const QHttpServerRequest &req);

private:
    int m_port;
    QHttpServer *m_server;

Q_SIGNALS:
    void serverStarted();
    void serverStopped();
    void requestReceived(const QHttpServerRequest *req, QHttpServerResponse *resp);
};

#endif // HTTPSERVER_H
