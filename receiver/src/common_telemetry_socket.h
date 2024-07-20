#ifndef CommonTelemetrySocket_H
#define CommonTelemetrySocket_H

#include <QObject>
#include <QUdpSocket>

class CommonTelemetrySocket : public QObject
{
    Q_OBJECT
public:
    explicit CommonTelemetrySocket(QObject *parent = nullptr);
    ~CommonTelemetrySocket();

public slots:
    void startListening(int port);
    void messageSend(const QString &message, const QString &host, int port);

signals:
    void messageReceived(const QString &message);

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket *udpSocket;
};

#endif // CommonTelemetrySocket_H
