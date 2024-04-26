#ifndef ENTRANCEHISTORY_H
#define ENTRANCEHISTORY_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>
#include <QVector>
#include <QSharedPointer>
#include "entrancerecord.h"

class EntranceHistory : public QObject
{
    Q_OBJECT

public:
    EntranceHistory(QString entranceHistoryPath, QObject *parent = nullptr);
    ~EntranceHistory();
    QVector<EntranceRecord *> getEntranceRecords(int maxRecords = 10);
    QJsonDocument convertToJson(const QVector<EntranceRecord *> &records);

Q_SIGNALS:
    void getEntranceRecordsRequested(const QJsonDocument &jsonDo);
    void sendAunthenticatedRFID(const QJsonDocument &jsonDoc);

public slots:
    void getEntranceRecordsHistory();
    void addEntranceRecord(EntranceRecord *record);
    void addAuthenticatedFID(const QJsonDocument &jsonDoc);

private:
    int m_lastRecordIndex = 0;
    QString m_entranceHistoryPath;
    QVector<EntranceRecord *> m_entranceRecords;
    void loadEntranceRecords();
    void saveEntranceRecords();
};

#endif // ENTRANCEHISTORY_H
