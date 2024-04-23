#include <QCoreApplication>
#include "httpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HttpServer server(8080);

    QObject::connect(&server, &HttpServer::serverStarted, []() {
        qDebug() << "Server started";
    });

    QObject::connect(&server, &HttpServer::serverStopped, []() {
        qDebug() << "Server stopped";
    });

    QObject::connect(&server, &HttpServer::requestReceived, [](const QHttpServerRequest *req, QHttpServerResponse *resp) {
        qDebug() << "Request received:" << req->url().toString();
        qDebug() << "Response sent:" << resp->statusCode();
    });

    server.startServer();

    return a.exec();
}
