// commonnetworkservice.h
#ifndef COMMONNETWORKSERVICE_H
#define COMMONNETWORKSERVICE_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QProcess>
#include <QProcessEnvironment>
#include <QString>
#include <QStringList>

class CommonNetworkService : public QObject {
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
