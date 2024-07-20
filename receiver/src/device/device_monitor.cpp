#include <gst/gst.h>
#include <json/json.h>
#include <iostream>
#include <string>

Json::Value capsToJson(GstCaps *caps)
{
    Json::Value capsArray(Json::arrayValue);

    for (guint i = 0; i < gst_caps_get_size(caps); ++i)
    {
        GstStructure *structure = gst_caps_get_structure(caps, i);
        gchar *capsString = gst_structure_to_string(structure);
        capsArray.append(capsString);
        g_free(capsString);
    }

    return capsArray;
}

Json::Value deviceToJson(GstDevice *device)
{
    Json::Value deviceJson;
    const gchar *deviceName = gst_device_get_display_name(device);
    deviceJson["Device"] = deviceName;

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
                deviceJson[name] = g_value_get_string(value);
            }
            else if (G_VALUE_HOLDS_INT(value))
            {
                deviceJson[name] = g_value_get_int(value);
            }
            else if (G_VALUE_HOLDS_BOOLEAN(value))
            {
                deviceJson[name] = g_value_get_boolean(value);
            }
            else if (G_VALUE_HOLDS_FLOAT(value))
            {
                deviceJson[name] = g_value_get_float(value);
            }
            else
            {
                deviceJson[name] = "Unsupported value type";
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

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    GstCommonDeviceMonitor *commonDeviceMonitor = gst_device_monitor_new();
    gst_device_monitor_add_filter(commonDeviceMonitor, "Video/Source", nullptr);
    // gst_device_monitor_add_filter(commonDeviceMonitor, "Audio/Source", nullptr);

    gst_device_monitor_start(commonDeviceMonitor);
    GList *devices = gst_device_monitor_get_devices(commonDeviceMonitor);
    Json::Value devicesArray(Json::arrayValue);

    for (GList *l = devices; l != nullptr; l = l->next)
    {
        GstDevice *device = GST_DEVICE(l->data);
        devicesArray.append(deviceToJson(device));
        gst_object_unref(device);
    }

    g_list_free(devices);
    gst_object_unref(commonDeviceMonitor);

    Json::StreamWriterBuilder writer;
    writer["emitUTF8"] = true;
    writer["indentation"] = "  ";

    std::string output = Json::writeString(writer, devicesArray);
    std::cout << output << std::endl;

    return 0;
}
