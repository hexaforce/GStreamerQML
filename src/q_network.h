// q_network.h
#ifndef Q_NETWORK_H
#define Q_NETWORK_H

#include <QObject>
#include <QString>
#include <QStringList>

class Q_Network : public QObject
{
    Q_OBJECT
public:
    explicit Q_Network(QObject *parent = nullptr);

public slots:
    QString getIwconfigOutput();
    QStringList getWifiDevices();
    QString getDeviceStatus(const QString &device);
    QString getDeviceSupport(const QString &device);

private:
    QStringList parseIwconfigOutput(const QString &output);
};

#endif // Q_NETWORK_H
