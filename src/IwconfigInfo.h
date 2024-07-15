// IwconfigInfo.h
#ifndef IWCONFIGINFO_H
#define IWCONFIGINFO_H

#include <QObject>
#include <QString>
#include <QStringList>

class IwconfigInfo : public QObject
{
    Q_OBJECT
public:
    explicit IwconfigInfo(QObject *parent = nullptr);

public slots:
    QString getIwconfigOutput();
    QStringList getWifiDevices();
    QString getDeviceStatus(const QString &device);
    QString getDeviceSupport(const QString &device);

private:
    QStringList parseIwconfigOutput(const QString &output);
};

#endif // IWCONFIGINFO_H
