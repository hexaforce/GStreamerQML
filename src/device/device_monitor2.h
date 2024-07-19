#ifndef COMMON_DEVICE_MONITOR_H
#define COMMON_DEVICE_MONITOR_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <gst/gst.h>

class CommonDeviceMonitor : public QObject {
    Q_OBJECT
public:
    explicit CommonDeviceMonitor(QObject *parent = nullptr);

    Q_INVOKABLE QString listDevices(const QString &deviceType);

private:
    QJsonArray capsToJson(GstCaps *caps);
    QJsonArray deviceToJson(GstElementFactory *factory);
};

#endif // COMMON_DEVICE_MONITOR_H
