#ifndef MONITORINGSYSTEMUSER_H
#define MONITORINGSYSTEMUSER_H

#include <QObject>
#include <QString>

class MonitoringSystemUser : public QObject
{
    Q_OBJECT

public:
    MonitoringSystemUser(QString username, QString password, QObject *parent = nullptr);
    ~MonitoringSystemUser();
    QString getUsername() const;
    bool validatePassword(const QString &password) const;

Q_SIGNALS:

private:
    QString m_username;
    QString m_password;
};

#endif // MONITORINGSYSTEMUSER_H
