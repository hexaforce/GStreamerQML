// q_network.h
#ifndef Q_NETWORK_H
#define Q_NETWORK_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QProcessEnvironment>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QProcessEnvironment>
class Q_Network : public QObject
{
    Q_OBJECT
public:
    explicit Q_Network(QObject *parent = nullptr);

public slots:
    QString getHostapdStatus();
    QString getDnsmasqStatus();
    QJsonArray parseIptablesChain(const QString &chainName, const QStringList &lines);
    QJsonObject getIptablesStatus();
    QString getCombinedStatus();

    QString getNetworkInfoAsJson();

    QString getIwconfigOutput();
    QStringList getWifiDevices();
    QString getDeviceStatus(const QString &device);
    QString getDeviceSupport(const QString &device);

private:
    QStringList parseIwconfigOutput(const QString &output);
    QString getSystemctlStatus(const QString &serviceName);
};

#endif // Q_NETWORK_H
