#ifndef WEBSOCKETRESPONSE_H
#define WEBSOCKETRESPONSE_H

#include <QObject>

class WebSocketResponse : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketResponse(QObject *parent = nullptr);

signals:
};

#endif // WEBSOCKETRESPONSE_H
