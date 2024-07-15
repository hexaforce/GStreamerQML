#include <gst/gst.h>
#include <gst/gstdevice.h>
#include <json/json.h>
#include <iostream>

std::string capsToString(GstCaps *caps) {
    std::vector<std::string> capsList;
    for (guint i = 0; i < gst_caps_get_size(caps); ++i) {
        GstStructure *structure = gst_caps_get_structure(caps, i);
        gchar *structureStr = gst_structure_to_string(structure);
        capsList.push_back(structureStr);
        g_free(structureStr);
    }
    Json::Value jsonCaps(Json::arrayValue);
    for (const auto &cap : capsList) {
        jsonCaps.append(cap);
    }
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, jsonCaps);
}

Json::Value deviceToJson(GstDevice *device) {
    Json::Value deviceInfo;
    deviceInfo["Device"] = gst_device_get_display_name(device);

    // Get properties
    GObject *obj = G_OBJECT(device);
    GParamSpec **props;
    guint nProps;
    props = g_object_class_list_properties(G_OBJECT_GET_CLASS(obj), &nProps);

    Json::Value properties(Json::objectValue);
    for (guint i = 0; i < nProps; ++i) {
        GParamSpec *prop = props[i];
        GValue value = G_VALUE_INIT;
        g_value_init(&value, prop->value_type);
        g_object_get_property(obj, prop->name, &value);
        if (G_VALUE_HOLDS_STRING(&value)) {
            properties[prop->name] = g_value_get_string(&value);
        } else if (G_VALUE_HOLDS_INT(&value)) {
            properties[prop->name] = g_value_get_int(&value);
        } else if (G_VALUE_HOLDS_FLOAT(&value)) {
            properties[prop->name] = g_value_get_float(&value);
        } else if (G_VALUE_HOLDS_BOOLEAN(&value)) {
            properties[prop->name] = g_value_get_boolean(&value);
        } else {
            gchar *valueStr = g_strdup_value_contents(&value);
            properties[prop->name] = valueStr;
            g_free(valueStr);
        }
        g_value_unset(&value);
    }
    g_free(props);
    deviceInfo["properties"] = properties;

    // Get caps information
    GstCaps *caps = gst_device_get_caps(device);
    if (caps) {
        deviceInfo["caps"] = capsToString(caps);
        gst_caps_unref(caps);
    }

    return deviceInfo;
}

void free_device_list(GList *deviceList) {
    g_list_free_full(deviceList, (GDestroyNotify) gst_object_unref);
}

int main(int argc, char *argv[]) {
    gst_init(&argc, &argv);

    GstDeviceMonitor *deviceMonitor = gst_device_monitor_new();
    gst_device_monitor_add_filter(deviceMonitor, "Audio/Source", NULL);

    GList *deviceList = gst_device_monitor_get_devices(deviceMonitor);
    Json::Value devices(Json::arrayValue);
    for (GList *l = deviceList; l != NULL; l = l->next) {
        GstDevice *device = GST_DEVICE(l->data);
        devices.append(deviceToJson(device));
    }
    free_device_list(deviceList);

    Json::StreamWriterBuilder writer;
    writer["indentation"] = "  "; // インデントを設定
    std::cout << Json::writeString(writer, devices) << std::endl;

    gst_object_unref(deviceMonitor);
    gst_deinit();

    return 0;
}
