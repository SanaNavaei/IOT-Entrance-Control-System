#ifndef ENTRANCESERVERCLOUD_H
#define ENTRANCESERVERCLOUD_H

#include <QObject>

class EntranceServerCloud : public QObject
{
    Q_OBJECT
public:
    explicit EntranceServerCloud(QObject *parent = nullptr);

signals:
};

#endif // ENTRANCESERVERCLOUD_H
