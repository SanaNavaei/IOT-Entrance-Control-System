#include "httpserver.h"

HttpServer::HttpServer(Authenticator *authenticator, const QHostAddress &address, quint16 port, QObject *parent)
    : QObject{parent}
{
    m_address = address;
    m_port = port;
    m_authenticator = authenticator;
    m_server = new QHttpServer{this};
    qDebug() << "Server Initialized";

    m_server->route("/", [this](const QHttpServerRequest &req)
                    { return this->handleRequest(req); });
}

void HttpServer::startServer()
{
    int ok = m_server->listen(m_address, m_port);
    if (!ok)
    {
        qDebug() << "Server could not start";
        return;
    }
    emit serverStarted(m_address, m_port);
}

void HttpServer::stopServer()
{
    m_server->disconnect();
    emit serverStopped();
}

QHttpServerResponse HttpServer::handleRequest(const QHttpServerRequest &req)
{
    QByteArray body = req.body();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
    QJsonObject jsonObj = jsonDoc.object();
    QString type = jsonObj.value("type").toString();
    if (type == "rfid")
    {
        QJsonObject dataObj = jsonObj.value("data").toObject();
        QString rfid = dataObj.value("tag").toString();
        bool isPermitted = m_authenticator->isPermitted(rfid);
        qDebug() << "RFID received: " << rfid << " isPermitted: " << isPermitted;
        EntranceRecord *record = new EntranceRecord(rfid, isPermitted);
        emit newEntraceRecord(record);

        QJsonObject response;
        response["permitted"] = isPermitted;
        QJsonDocument responseDoc(response);
        QByteArray responseBytes = responseDoc.toJson(QJsonDocument::Compact);
        QHttpServerResponse::StatusCode statusCode = isPermitted ? QHttpServerResponse::StatusCode::Ok : QHttpServerResponse::StatusCode::Forbidden;
        QHttpServerResponse resp(responseBytes, statusCode);
        emit requestReceived(&req, &resp);
        return resp;
    }
    else
    {
        QJsonObject response;
        response["message"] = "Invalid request";
        QJsonDocument responseDoc(response);
        QByteArray responseBytes = responseDoc.toJson(QJsonDocument::Compact);
        QHttpServerResponse resp(responseBytes, QHttpServerResponse::StatusCode::BadRequest);
        emit requestReceived(&req, &resp);
        return resp;
    }
}

HttpServer::~HttpServer()
{
    m_server->deleteLater();
}
