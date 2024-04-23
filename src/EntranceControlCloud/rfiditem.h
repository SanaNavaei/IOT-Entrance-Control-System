#ifndef RFIDITEM_H
#define RFIDITEM_H

#include <QObject>

class RFIDItem : public QObject
{
    Q_OBJECT
public:
    explicit RFIDItem(QObject *parent = nullptr);

signals:
};

#endif // RFIDITEM_H
