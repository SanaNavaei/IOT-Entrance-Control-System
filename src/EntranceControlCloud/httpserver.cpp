#include "httpserver.h"

HttpServer::HttpServer(int port, QObject *parent)
    : QObject{parent}
{
    m_port = port;
    m_server = new QHttpServer{this};
    qDebug() << "Server Initialized";

    m_server->route("/", [this] (const QHttpServerRequest &req) {
        return this->handleRequest(req);
    });
}

void HttpServer::startServer()
{
    int ok = m_server->listen(QHostAddress::LocalHost, m_port);
    if (!ok) {
        qDebug() << "Server could not start";
        return;
    }
    qDebug() << "Server listening on port" << m_port;
    emit serverStarted();
}

void HttpServer::stopServer()
{
    m_server->disconnect();
    qDebug() << "Server stopped";
    emit serverStopped();
}

QHttpServerResponse HttpServer::handleRequest(const QHttpServerRequest &req)
{
    QJsonObject response;
    response["message"] = "Boula BACKKKK";
    QJsonDocument responseDoc(response);
    QByteArray responseBytes = responseDoc.toJson(QJsonDocument::Compact);
    QHttpServerResponse resp(responseBytes, QHttpServerResponse::StatusCode::Ok);
    emit requestReceived(&req, &resp);
    return resp;
}

HttpServer::~HttpServer()
{
    m_server->deleteLater();
}
