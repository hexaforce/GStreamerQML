#ifndef DEVICE_MONITOR_H
#define DEVICE_MONITOR_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <gst/gst.h>

class DeviceMonitor : public QObject {
    Q_OBJECT
public:
    explicit DeviceMonitor(QObject *parent = nullptr);

    Q_INVOKABLE QString listDevices(const QString &deviceType);

private:
    QJsonArray capsToJson(GstCaps *caps);
    QJsonArray deviceToJson(GstElementFactory *factory);
};

#endif // DEVICE_MONITOR_H
