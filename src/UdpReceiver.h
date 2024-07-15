#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>
#include <QUdpSocket>

class UdpReceiver : public QObject
{
    Q_OBJECT
public:
    explicit UdpReceiver(QObject *parent = nullptr);
    ~UdpReceiver();

public slots:
    void startListening(int port);

signals:
    void messageReceived(const QString &message);

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket *udpSocket;
};

#endif // UDPRECEIVER_H
