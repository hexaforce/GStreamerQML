#include "device_monitor.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>

DeviceMonitor::DeviceMonitor(QObject *parent) : QObject(parent)
{
    gst_init(nullptr, nullptr);
}

QJsonArray DeviceMonitor::capsToJson(GstCaps *caps)
{
    QJsonArray capsArray;

    for (guint i = 0; i < gst_caps_get_size(caps); ++i)
    {
        GstStructure *structure = gst_caps_get_structure(caps, i);
        gchar *capsString = gst_structure_to_string(structure);
        capsArray.append(QString::fromUtf8(capsString));
        g_free(capsString);
    }

    return capsArray;
}

QJsonObject DeviceMonitor::deviceToJson(GstDevice *device)
{
    QJsonObject deviceJson;
    const gchar *deviceName = gst_device_get_display_name(device);
    deviceJson["Device"] = QString::fromUtf8(deviceName);

    GstStructure *properties = gst_device_get_properties(device);
    if (properties)
    {
        const gchar *name;
        const GValue *value;

        for (guint i = 0; i < gst_structure_n_fields(properties); ++i)
        {
            name = gst_structure_nth_field_name(properties, i);
            value = gst_structure_get_value(properties, name);

            if (G_VALUE_HOLDS_STRING(value))
            {
                deviceJson[QString::fromUtf8(name)] = QString::fromUtf8(g_value_get_string(value));
            }
            else if (G_VALUE_HOLDS_INT(value))
            {
                deviceJson[QString::fromUtf8(name)] = g_value_get_int(value);
            }
            else if (G_VALUE_HOLDS_BOOLEAN(value))
            {
                deviceJson[QString::fromUtf8(name)] = g_value_get_boolean(value);
            }
            else if (G_VALUE_HOLDS_FLOAT(value))
            {
                deviceJson[QString::fromUtf8(name)] = g_value_get_float(value);
            }
            else
            {
                deviceJson[QString::fromUtf8(name)] = "Unsupported value type";
            }
        }

        gst_structure_free(properties);
    }

    GstCaps *caps = gst_device_get_caps(device);
    if (caps)
    {
        deviceJson["caps"] = capsToJson(caps);
        gst_caps_unref(caps);
    }

    return deviceJson;
}

QString DeviceMonitor::listDevices(const QString &deviceType)
{
    GstDeviceMonitor *deviceMonitor = gst_device_monitor_new();
    QString filter = "Source/" + deviceType;
    gst_device_monitor_add_filter(deviceMonitor, filter.toStdString().c_str(), nullptr);

    gst_device_monitor_start(deviceMonitor);
    GList *devices = gst_device_monitor_get_devices(deviceMonitor);
    QJsonArray devicesArray;

    for (GList *l = devices; l != nullptr; l = l->next)
    {
        GstDevice *device = GST_DEVICE(l->data);
        devicesArray.append(deviceToJson(device));
        gst_object_unref(device);
    }

    g_list_free(devices);
    gst_object_unref(deviceMonitor);

    QJsonDocument doc(devicesArray);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Indented));
}
