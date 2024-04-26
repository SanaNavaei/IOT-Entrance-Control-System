#ifndef MONITORINGSYSTEMAUTHENTICATOR_H
#define MONITORINGSYSTEMAUTHENTICATOR_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>
#include <QVector>
#include <QSharedPointer>
#include "monitoringsystemuser.h"

class MonitoringSystemAuthenticator : public QObject
{
    Q_OBJECT
public:
    MonitoringSystemAuthenticator(const QString &systemUserPath, QObject *parent = nullptr);
    ~MonitoringSystemAuthenticator();

Q_SIGNALS:
    void authenticated();
    void unauthenticated();

public Q_SLOTS:
    void authenticate(const QString &username, const QString &password);

private:
    void loadMonitoringSystemUsers();

    QString m_systemUserPath;
    QVector<MonitoringSystemUser *> m_monitoringSystemUsers;
};

#endif // MONITORINGSYSTEMAUTHENTICATOR_H
