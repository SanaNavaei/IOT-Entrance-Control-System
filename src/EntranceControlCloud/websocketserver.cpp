#include "websocketserver.h"

WebSocketServer::WebSocketServer(const QHostAddress &address, quint16 port, QObject *parent) : QObject(parent)
{
    m_address = address;
    m_port = port;
    m_server = new QWebSocketServer(QStringLiteral("WebSocket Server"), QWebSocketServer::NonSecureMode, this);
    m_server->listen(m_address, m_port);
    connect(m_server, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
    qDebug() << "WebSocket server started on port" << port;
}

void WebSocketServer::onNewConnection()
{
    QWebSocket *socket = m_server->nextPendingConnection();
    connect(socket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(socket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);
}

void WebSocketServer::processTextMessage(const QString &message)
{
    qDebug() << "Message received";
    m_client_socket = qobject_cast<QWebSocket *>(sender());

    QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();

    if (m_client_socket)
    {
        if (jsonObj.value("type").toString() == "history")
        {
            if (!m_isAuthenticated)
            {
                qDebug() << "Not authenticated";
                return;
            }
            qDebug() << "History requested";
            emit historyRequested();
        }
        else if (jsonObj.value("type").toString() == "authenticate")
        {
            qDebug() << "Authenticate requested";
            QJsonValue dataValue = jsonObj.value("data");
            QJsonObject dataObj = dataValue.toObject();
            QString username = dataObj.value("username").toString();
            QString password = dataObj.value("password").toString();
            emit authenticateRequested(username, password);
        }
        // else if (jsonObj.value("type").toString() == "rfid")
        // {
        //     qDebug() << "RFID tag requested";
        //     emit authenticateRFID(jsonDoc);
        // }
    }
}

void WebSocketServer::sendHistory(const QJsonDocument &jsonDoc)
{
    if (m_client_socket)
    {
        QByteArray jsonString = jsonDoc.toJson();
        QString jsonStringStr = QString::fromUtf8(jsonString);
        m_client_socket->sendTextMessage(jsonStringStr);
    }
}

void WebSocketServer::sendAuthenticate()
{
    QJsonObject jsonObj;
    jsonObj.insert("isAuthenticated", "true");
    QJsonDocument jsonDoc(jsonObj);
    m_isAuthenticated = true;
    qDebug() << "Authenticated:" << m_isAuthenticated;
    QByteArray jsonString = jsonDoc.toJson();
    if (m_client_socket)
    {
        QString jsonStringStr = QString::fromUtf8(jsonString);
        m_client_socket->sendTextMessage(jsonStringStr);
    }
}

void WebSocketServer::sendUnauthenticate()
{
    QJsonObject jsonObj;
    jsonObj.insert("isAuthenticated", "false");
    QJsonDocument jsonDoc(jsonObj);
    m_isAuthenticated = false;
    qDebug() << "Authenticated:" << m_isAuthenticated;
    QByteArray jsonString = jsonDoc.toJson();
    if (m_client_socket)
    {
        QString jsonStringStr = QString::fromUtf8(jsonString);
        m_client_socket->sendTextMessage(jsonStringStr);
    }
}

void WebSocketServer::sendEntranceRecord(EntranceRecord *record)
{
    QJsonObject jsonObj;
    jsonObj.insert("username", record->rfidTag());
    jsonObject.insert("time", record->entranceTime().toString("hh:mm"));
    jsonObject.insert("date", record->entranceTime().toString("MM/dd/yyyy"));
    jsonObj.insert("permitted", record->isPermitted());
    QJsonDocument jsonDoc(jsonObj);
    QByteArray jsonString = jsonDoc.toJson();
    if (m_client_socket)
    {
        QString jsonStringStr = QString::fromUtf8(jsonString);
        m_client_socket->sendTextMessage(jsonStringStr);
    }
}

void WebSocketServer::socketDisconnected()
{
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    if (socket)
    {
        m_isAuthenticated = false;
        // emit clientDisconnected(socket);
        socket->deleteLater();
    }
}
