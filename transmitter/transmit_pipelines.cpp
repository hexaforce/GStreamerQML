#include "transmit_pipelines.h"

GstStateChangeReturn setupVideoTransmitPipeline(GstElement *pipeline, const char *device, const char *host, int port, CodecType codecType) {
    // ビデオ送信パイプラインのセットアップコード
    // codecTypeに基づいてH264またはH265を設定
    // device, host, portの使用
    // 例:
    // GstElement *src = gst_element_factory_make("v4l2src", "src");
    // GstElement *enc = gst_element_factory_make("x264enc", "enc");
    // GstElement *pay = gst_element_factory_make("rtph264pay", "pay");
    // GstElement *sink = gst_element_factory_make("udpsink", "sink");
    // gst_bin_add_many(GST_BIN(pipeline), src, enc, pay, sink, NULL);
    // gst_element_link_many(src, enc, pay, sink, NULL);
    // gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // 簡略化のための例
    return GST_STATE_CHANGE_SUCCESS;
}

GstStateChangeReturn setupAudioTransmitPipeline(GstElement *pipeline, const char *device, const char *host, int port) {
    // オーディオ送信パイプラインのセットアップコード
    // device, host, portの使用
    // 例:
    // GstElement *src = gst_element_factory_make("alsasrc", "src");
    // GstElement *enc = gst_element_factory_make("opusenc", "enc");
    // GstElement *pay = gst_element_factory_make("rtpopuspay", "pay");
    // GstElement *sink = gst_element_factory_make("udpsink", "sink");
    // gst_bin_add_many(GST_BIN(pipeline), src, enc, pay, sink, NULL);
    // gst_element_link_many(src, enc, pay, sink, NULL);
    // gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // 簡略化のための例
    return GST_STATE_CHANGE_SUCCESS;
}
