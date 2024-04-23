#include "entrancerecord.h"

EntranceRecord::EntranceRecord(QString rfidTag, QObject *parent)
    : QObject{parent}
{
    m_rfidTag = rfidTag;
    m_entranceTime = QDateTime::currentDateTime();
    m_isPermitted = false;
}

EntranceRecord::EntranceRecord(QString rfidTag, QDateTime entranceTime, QObject *parent)
    : QObject{parent}
{
    m_rfidTag = rfidTag;
    m_entranceTime = entranceTime;
    m_isPermitted = false;
}

EntranceRecord::~EntranceRecord()
{
}

QString EntranceRecord::rfidTag() const
{
    return m_rfidTag;
}

QDateTime EntranceRecord::entranceTime() const
{
    return m_entranceTime;
}

bool EntranceRecord::isPermitted() const
{
    return m_isPermitted;
}

void EntranceRecord::permit()
{
    m_isPermitted = true;
    emit permissionChanged(true);
}

void EntranceRecord::deny()
{
    m_isPermitted = false;
    emit permissionChanged(false);
}
