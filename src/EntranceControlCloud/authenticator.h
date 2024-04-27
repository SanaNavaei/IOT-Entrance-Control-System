#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <QObject>
#include <Qvector>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include "entrancerecord.h"
#include "entrancehistory.h"
class Authenticator : public QObject
{
    Q_OBJECT

public:
    Authenticator(QString permittedTagsPath, QObject *parent = nullptr);
    ~Authenticator();
    bool isPermitted(const QString &tag);

// Q_SIGNALS:
//     void authenticatedRFID(const QJsonDocument &jsonDoc);
//     void unauthenticatedRFID();

// public slots:
//     void authenticateRFID(const QJsonDocument &jsonDoc);

private:
    QString m_permittedTagsPath;
    QVector<QString> m_permittedTags;
    void loadPermittedTags();
    void savePermittedTags();
};

#endif // AUTHENTICATOR_H
