// commonnetworkservice.h
#ifndef COMMONNETWORKSERVICE_H
#define COMMONNETWORKSERVICE_H

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

class CommonNetworkService : public QObject
{
    Q_OBJECT
public:
    explicit CommonNetworkService(QObject *parent = nullptr);

public slots:

    QJsonArray parseIptablesChain(const QString &chainName, const QStringList &lines);
    QJsonObject getIptablesStatus();
    QString getCombinedStatus();

    QString getNetworkInfoAsJson();

private:
    QString readConfFile(const QString &filePath);
    QStringList parseIwconfigOutput(const QString &output);
    QString getSystemctlStatus(const QString &serviceName);
};

#endif // COMMONNETWORKSERVICE_H
