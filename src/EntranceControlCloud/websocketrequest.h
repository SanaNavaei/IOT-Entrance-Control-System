#ifndef WEBSOCKETREQUEST_H
#define WEBSOCKETREQUEST_H

#include <QObject>

class WebSocketRequest : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketRequest(QObject *parent = nullptr);

signals:
};

#endif // WEBSOCKETREQUEST_H
