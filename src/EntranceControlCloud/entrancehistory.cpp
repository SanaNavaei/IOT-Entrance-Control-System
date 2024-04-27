#include "entrancehistory.h"

EntranceHistory::EntranceHistory(QString entranceHistoryPath, QObject *parent)
    : QObject{parent}
{
    m_entranceHistoryPath = entranceHistoryPath;
    loadEntranceRecords();
}

EntranceHistory::~EntranceHistory()
{
    qDebug() << "Destructing EntranceHistory";
    saveEntranceRecords();
    for (int i = 0; i < m_entranceRecords.size(); i++)
    {
        delete m_entranceRecords.at(i);
    }
}

void EntranceHistory::addEntranceRecord(EntranceRecord *record)
{
    m_entranceRecords.push_back(record);

    QFile file(m_entranceHistoryPath);
    if (!file.exists())
    {
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Failed to create file: " << m_entranceHistoryPath;
            return;
        }

        QTextStream out(&file);
        QJsonObject obj;
        out << "[\n";
        obj.insert("tag", record->rfidTag());
        obj.insert("timestamp", record->entranceTime().toString(Qt::ISODate));
        obj.insert("permitted", record->isPermitted());
        QJsonDocument doc(obj);
        out << doc.toJson(QJsonDocument::Indented);
        out << "]";
        file.close();
    }
    else if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "Failed to open file: " << m_entranceHistoryPath;
        return;
    }

    else
    {
        file.seek(file.size() - 3);
        QTextStream out(&file);
        out << ",\n";
        QJsonObject obj;
        obj.insert("tag", record->rfidTag());
        obj.insert("timestamp", record->entranceTime().toString(Qt::ISODate));
        obj.insert("permitted", record->isPermitted());
        QJsonDocument doc(obj);
        out << doc.toJson(QJsonDocument::Indented);
        out << "]";
        file.close();
    }
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
    QJsonDocument entranceRecords = convertToJson(getEntranceRecords(10));
    emit getEntranceRecordsRequested(entranceRecords);
}

QJsonDocument EntranceHistory::convertToJson(const QVector<EntranceRecord *> &records)
{
    QJsonArray jsonArray;
    for (const EntranceRecord *record : records)
    {
        QJsonObject jsonObject;
        jsonObject.insert("username", record->rfidTag());
        jsonObject.insert("time", record->entranceTime().toString("hh:mm"));
        jsonObject.insert("date", record->entranceTime().toString("MM/dd/yyyy"));
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
    qDebug() << "Saving entrance records";
    QFile file(m_entranceHistoryPath);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to open file" << m_entranceHistoryPath;
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
