#include <QCoreApplication>
#include "httpserver.h"
#include "websocketserver.h"
#include "monitoringsystemauthenticator.h"
#include "entrancehistory.h"
#include "authenticator.h"

const QString USERS_FILE_PATH = "data/users.json";
const QString ENTRANCE_HISTORY_FILE_PATH = "data/entrance_history.json";
const QString RFID_TAGS_FILE_PATH = "data/rfid_tags.json";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "App path : " << QCoreApplication::applicationDirPath();
    HttpServer server(5050);
    WebSocketServer wsServer(QHostAddress::LocalHost, 5051);
    MonitoringSystemAuthenticator monitoringSystemAuthenticator(USERS_FILE_PATH);
    EntranceHistory entranceHistory(ENTRANCE_HISTORY_FILE_PATH);
    Authenticator authenticator(RFID_TAGS_FILE_PATH);

    QObject::connect(&server, &HttpServer::serverStarted, []()
                     { qDebug() << "Server started"; });

    QObject::connect(&server, &HttpServer::serverStopped, []()
                     { qDebug() << "Server stopped"; });

    QObject::connect(&server, &HttpServer::requestReceived, [](const QHttpServerRequest *req, QHttpServerResponse *resp)
                     {
        qDebug() << "Request received:" << req->url().toString();
        qDebug() << "Response sent:" << resp->statusCode(); });

    QObject::connect(&wsServer, &WebSocketServer::authenticateRequested, &monitoringSystemAuthenticator, &MonitoringSystemAuthenticator::authenticate);
    QObject::connect(&monitoringSystemAuthenticator, &MonitoringSystemAuthenticator::authenticated, &wsServer, &WebSocketServer::sendAuthenticate);
    QObject::connect(&monitoringSystemAuthenticator, &MonitoringSystemAuthenticator::unauthenticated, &wsServer, &WebSocketServer::sendUnauthenticate);
    QObject::connect(&wsServer, &WebSocketServer::historyRequested, &entranceHistory, &EntranceHistory::getEntranceRecordsHistory);
    QObject::connect(&entranceHistory, &EntranceHistory::getEntranceRecordsRequested, &wsServer, &WebSocketServer::sendHistory);

    QObject::connect(&wsServer, &WebSocketServer::authenticateRFID, &authenticator, &Authenticator::authenticateRFID);
    QObject::connect(&authenticator, &Authenticator::authenticatedRFID, &entranceHistory, &EntranceHistory::addAuthenticatedFID);
    QObject::connect(&entranceHistory, &EntranceHistory::sendAunthenticatedRFID, &wsServer, &WebSocketServer::sendAunthenticatedRFID);
    QObject::connect(&authenticator, &Authenticator::unauthenticatedRFID, &wsServer, &WebSocketServer::sendUnauthenticatedRFID);
    server.startServer();

    return a.exec();
}
