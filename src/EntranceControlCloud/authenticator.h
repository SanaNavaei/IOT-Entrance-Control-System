#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class Authenticator : public QObject
{
    Q_OBJECT

public:
    Authenticator(QString permittedTagsPath, QObject *parent = nullptr);
    ~Authenticator();
    bool isPermitted(const QString &tag);

Q_SIGNALS:

private:
    QString m_permittedTagsPath;    
    QStringList m_permittedTags;
    void loadPermittedTags();
    void savePermittedTags();
};

#endif // AUTHENTICATOR_H
