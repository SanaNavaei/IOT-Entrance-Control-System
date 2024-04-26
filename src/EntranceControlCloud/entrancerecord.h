#ifndef ENTRANCERECORD_H
#define ENTRANCERECORD_H

#include <QObject>
#include <QDateTime>
#include <QString>

class EntranceRecord : public QObject
{
    Q_OBJECT

public:
    EntranceRecord(QString rfidTag, QObject *parent = nullptr);
    EntranceRecord(QString rfidTag, QDateTime entranceTime, QObject *parent = nullptr);
    EntranceRecord(QString rfidTag, bool isPermitted, QObject *parent = nullptr);
    EntranceRecord(QString rfidTag, QString entranceTime, QString isPermitted, QObject *parent = nullptr); // (1
    ~EntranceRecord();
    QString rfidTag() const;
    QDateTime entranceTime() const;
    bool isPermitted() const;
    void permit();
    void deny();

Q_SIGNALS:
    void permissionChanged(bool isPermitted);

private:
    QString m_rfidTag;
    QDateTime m_entranceTime;
    bool m_isPermitted;
};

#endif // ENTRANCERECORD_H
