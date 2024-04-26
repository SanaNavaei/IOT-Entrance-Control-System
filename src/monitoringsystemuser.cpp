#include "monitoringsystemuser.h"

MonitoringSystemUser::MonitoringSystemUser(QString username, QString password, QObject *parent)
    : QObject{parent}
{
    m_username = username;
    m_password = password;
}

MonitoringSystemUser::~MonitoringSystemUser()
{
}

QString MonitoringSystemUser::getUsername() const
{
    return m_username;
}

bool MonitoringSystemUser::validatePassword(const QString &password) const
{
    return m_password == password;
}
