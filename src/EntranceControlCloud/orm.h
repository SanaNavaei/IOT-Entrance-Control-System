#ifndef ORM_H
#define ORM_H

#include <QObject>

class ORM : public QObject
{
    Q_OBJECT
public:
    explicit ORM(QObject *parent = nullptr);

signals:
};

#endif // ORM_H
