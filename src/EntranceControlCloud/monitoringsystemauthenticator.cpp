#include "monitoringsystemauthenticator.h"

MonitoringSystemAuthenticator::MonitoringSystemAuthenticator(const QString &systemUserPath, QObject *parent) : QObject(parent)
{
    m_systemUserPath = systemUserPath;
    loadMonitoringSystemUsers();
}

MonitoringSystemAuthenticator::~MonitoringSystemAuthenticator()
{
    qDebug() << "Destructing MonitoringSystemAuthenticator";
    for (int i = 0; i < m_monitoringSystemUsers.size(); i++)
    {
        delete m_monitoringSystemUsers.at(i);
    }
    m_monitoringSystemUsers.clear();
}

void MonitoringSystemAuthenticator::loadMonitoringSystemUsers()
{
    QFile file(m_systemUserPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open file" << m_systemUserPath;
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (jsonDoc.isNull())
    {
        qDebug() << "Invalid JSON data";
        return;
    }

    QJsonArray jsonArray = jsonDoc.array();
    for (const QJsonValue &value : jsonArray)
    {
        QJsonObject jsonObj = value.toObject();
        QJsonObject dataObj = jsonObj.value("data").toObject();
        QString username = dataObj.value("username").toString();
        QString password = dataObj.value("password").toString();

        MonitoringSystemUser *user = new MonitoringSystemUser(username, password);
        m_monitoringSystemUsers.append(user);
    }
}

void MonitoringSystemAuthenticator::authenticate(const QString &username, const QString &password)
{
    for (int i = 0; i < m_monitoringSystemUsers.size(); i++)
    {
        if (m_monitoringSystemUsers.at(i)->getUsername() == username && m_monitoringSystemUsers.at(i)->validatePassword(password))
        {
            emit authenticated();
            return;
        }
    }
    emit unauthenticated();
}
