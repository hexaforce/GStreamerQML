#include <gst/gst.h>
#include <json/json.h>
#include <iostream>
#include <string>

// GstCapsをJSONに変換する関数
Json::Value capsToJson(GstCaps *caps) {
    Json::Value capsArray(Json::arrayValue);

    for (guint i = 0; i < gst_caps_get_size(caps); ++i) {
        GstStructure *structure = gst_caps_get_structure(caps, i);
        gchar *capsString = gst_structure_to_string(structure);
        capsArray.append(capsString);
        g_free(capsString);
    }

    return capsArray;
}

// GstElementFactoryからデバイス情報をJSONに変換する関数
Json::Value deviceToJson(GstElementFactory *factory) {
    Json::Value deviceJson;
    const gchar *deviceName = gst_plugin_feature_get_name(GST_PLUGIN_FEATURE(factory));
    deviceJson["Device"] = deviceName;

    // GstStaticPadTemplateを取得
    const GList *padTemplates = gst_element_factory_get_static_pad_templates(factory);

    // すべてのパッドテンプレートについてループ
    for (const GList *iter = padTemplates; iter; iter = iter->next) {
        const GstStaticPadTemplate *padTemplate = static_cast<const GstStaticPadTemplate*>(iter->data);
        if (padTemplate->direction == GST_PAD_SRC) { // ソースパッドのみを対象
            GstCaps *caps = gst_static_pad_template_get_caps(const_cast<GstStaticPadTemplate*>(padTemplate));
            if (caps) {
                deviceJson["caps"] = capsToJson(caps);
                gst_caps_unref(caps);
            }
        }
    }

    return deviceJson;
}

int main(int argc, char *argv[]) {
    gst_init(&argc, &argv);

    GstRegistry *registry = gst_registry_get();
    GList *features = gst_registry_get_feature_list(registry, GST_TYPE_ELEMENT_FACTORY);

    Json::Value devicesArray(Json::arrayValue);

    for (GList *l = features; l != nullptr; l = l->next) {
        GstElementFactory *factory = GST_ELEMENT_FACTORY(l->data);

        // オーディオソースクラスを持つ要素をフィルタリング
        if (g_strrstr(gst_element_factory_get_klass(factory), "Source/Audio")) {
            devicesArray.append(deviceToJson(factory));
        }
    }

    g_list_free(features);

    Json::StreamWriterBuilder writer;
    std::string output = Json::writeString(writer, devicesArray);
    std::cout << output << std::endl;

    gst_deinit();
    return 0;
}
