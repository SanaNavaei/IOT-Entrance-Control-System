#include "entrancehistory.h"

EntranceHistory::EntranceHistory(QString entranceHistoryPath, QObject *parent)
    : QObject{parent}
{
    m_entranceHistoryPath = entranceHistoryPath;
    loadEntranceRecords();
}

EntranceHistory::~EntranceHistory()
{
    saveEntranceRecords();
}

void EntranceHistory::addEntranceRecord(EntranceRecord *record)
{
    m_entranceRecords.push_back(record);
}

QVector<EntranceRecord *> EntranceHistory::getEntranceRecords(int maxRecords)
{
    QVector<EntranceRecord *> records;
    for (int i = m_entranceRecords.size() - 1; i >= 0 && records.size() < maxRecords; --i)
    {
        records.append(m_entranceRecords.at(i));
    }
    return records;
}

void EntranceHistory::getEntranceRecordsHistory()
{
    qDebug() << "Entrance history requested";
    QJsonDocument entranceRecords = convertToJson(getEntranceRecords(10));
    qDebug() << entranceRecords.toJson(QJsonDocument::Compact);
    emit getEntranceRecordsRequested(entranceRecords);
}

void EntranceHistory::addAuthenticatedFID(const QJsonDocument &jsonDoc)
{
    qDebug() << "Add Authenticated RFID";
    QJsonObject jsonObj = jsonDoc.object();
    QString rfidTag = jsonObj.value("tag").toString();
    QDateTime date = QDateTime::currentDateTime();
    EntranceRecord *record = new EntranceRecord(rfidTag, date);
    addEntranceRecord(record);
    QJsonObject responseObj;
    responseObj.insert("tag", rfidTag);
    responseObj.insert("timestamp", date.toString());
    responseObj.insert("permitted", "true");
    QJsonDocument responseDoc(responseObj);
    emit sendAunthenticatedRFID(responseDoc);
}

QJsonDocument EntranceHistory::convertToJson(const QVector<EntranceRecord *> &records)
{
    QJsonArray jsonArray;
    for (const EntranceRecord *record : records)
    {
        QJsonObject jsonObject;
        jsonObject.insert("tag", record->rfidTag());
        jsonObject.insert("timestamp", record->entranceTime().toString(Qt::ISODate));
        jsonObject.insert("permitted", record->isPermitted());
        jsonArray.append(jsonObject);
    }

    return QJsonDocument(jsonArray);
}

void EntranceHistory::loadEntranceRecords()
{
    QFile file(m_entranceHistoryPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open file" << m_entranceHistoryPath;
        return;
    }

    QTextStream in(&file);
    QString jsonData = in.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    if (!doc.isArray())
    {
        qWarning() << "Invalid JSON array";
        return;
    }

    QJsonArray jsonArray = doc.array();

    for (const QJsonValue &value : jsonArray)
    {
        QJsonObject obj = value.toObject();
        QString tag = obj.value("tag").toString();
        QString timestamp = obj.value("timestamp").toString();
        QString permitted = obj.value("permitted").toString();

        EntranceRecord *record = new EntranceRecord(tag, timestamp, permitted);
        m_entranceRecords.append(record);
    }
}

void EntranceHistory::saveEntranceRecords()
{
    QFile file(m_entranceHistoryPath);
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "Could not open entrance history file for writing";
        return;
    }

    QTextStream out(&file);
    for (const EntranceRecord *record : qAsConst(m_entranceRecords))
    {
        QJsonObject obj;
        obj.insert("tag", record->rfidTag());
        obj.insert("timestamp", record->entranceTime().toString(Qt::ISODate));
        obj.insert("permitted", record->isPermitted());
        QJsonDocument doc(obj);
        out << doc.toJson(QJsonDocument::Compact) << '\n';
    }

    file.close();
}
