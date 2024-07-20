#include "common_device_monitor.h"

#include <QJsonDocument>
#include <QJsonObject>

CommonDeviceMonitor::CommonDeviceMonitor(QObject *parent) : QObject(parent) { gst_init(nullptr, nullptr); }

QJsonArray CommonDeviceMonitor::capsToJson(GstCaps *caps) {
  QJsonArray capsArray;

  for (guint i = 0; i < gst_caps_get_size(caps); ++i) {
    GstStructure *structure = gst_caps_get_structure(caps, i);
    gchar *capsString = gst_structure_to_string(structure);
    capsArray.append(QString(capsString));
    g_free(capsString);
  }

  return capsArray;
}

QJsonArray CommonDeviceMonitor::deviceToJson(GstElementFactory *factory) {
  QJsonArray devicesArray;
  QJsonObject deviceJson;
  const gchar *deviceName = gst_plugin_feature_get_name(GST_PLUGIN_FEATURE(factory));
  deviceJson["Device"] = QString(deviceName);

  const GList *padTemplates = gst_element_factory_get_static_pad_templates(factory);

  for (const GList *iter = padTemplates; iter; iter = iter->next) {
    const GstStaticPadTemplate *padTemplate = static_cast<const GstStaticPadTemplate *>(iter->data);
    if (padTemplate->direction == GST_PAD_SRC) {
      GstCaps *caps = gst_static_pad_template_get_caps(const_cast<GstStaticPadTemplate *>(padTemplate));
      if (caps) {
        deviceJson["caps"] = capsToJson(caps);
        gst_caps_unref(caps);
      }
    }
  }

  devicesArray.append(deviceJson);

  return devicesArray;
}

QString CommonDeviceMonitor::listDevices(const QString &deviceType) {
  GstRegistry *registry = gst_registry_get();
  GList *features = gst_registry_get_feature_list(registry, GST_TYPE_ELEMENT_FACTORY);

  QJsonArray devicesArray;

  for (GList *l = features; l != nullptr; l = l->next) {
    GstElementFactory *factory = GST_ELEMENT_FACTORY(l->data);

    if (g_strrstr(gst_element_factory_get_klass(factory), ("Source/" + deviceType.toStdString()).c_str())) {
      devicesArray.append(deviceToJson(factory));
    }
  }

  g_list_free(features);

  QJsonDocument doc(devicesArray);
  return QString(doc.toJson(QJsonDocument::Compact));
}
