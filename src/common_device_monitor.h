#ifndef COMMON_DEVICE_MONITOR_H
#define COMMON_DEVICE_MONITOR_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <gst/gst.h>

class CommonDeviceMonitor : public QObject
{
    Q_OBJECT
public:
    explicit CommonDeviceMonitor(QObject *parent = nullptr);

    // Function to list devices and return JSON representation
    Q_INVOKABLE QString listDevices(const QString &deviceType);

private:
    // Function to convert GstCaps to JSON
    QJsonArray capsToJson(GstCaps *caps);

    // Function to convert GstDevice to JSON
    QJsonObject deviceToJson(GstDevice *device);
};

#endif // COMMON_DEVICE_MONITOR_H
