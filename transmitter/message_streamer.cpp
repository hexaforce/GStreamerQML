#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include "../message/video_data.pb.h"  // protocで生成されたヘッダー

void sendVideoData(const VideoFrame& frame, const char* host, gint port) {
    // GStreamer初期化
    gst_init(NULL, NULL);

    // パイプライン作成
    GstElement *pipeline = gst_pipeline_new("video-send-pipeline");
    GstElement *appsrc = gst_element_factory_make("appsrc", "video-src");
    GstElement *tcpclientsink = gst_element_factory_make("tcpclientsink", "tcp-sink");

    // パイプラインに要素を追加
    gst_bin_add_many(GST_BIN(pipeline), appsrc, tcpclientsink, NULL);

    // 要素をリンク
    if (!gst_element_link(appsrc, tcpclientsink)) {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return;
    }

    // tcpclientsinkのプロパティ設定
    g_object_set(tcpclientsink, "host", host, "port", port, NULL);

    // プロトコルバッファをシリアライズ
    std::string serializedData;
    frame.SerializeToString(&serializedData);

    // GStreamerバッファを作成
    GstBuffer *buffer = gst_buffer_new_allocate(NULL, serializedData.size(), NULL);
    gst_buffer_fill(buffer, 0, serializedData.data(), serializedData.size());

    // バッファをappsrcにプッシュ
    gst_app_src_push_buffer(GST_APP_SRC(appsrc), buffer);

    // パイプラインを再生状態に設定
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // メインループ
    g_main_loop_run(g_main_loop_new(NULL, FALSE));

    // クリーンアップ
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
}

int main(int argc, char *argv[]) {
    // サンプルビデオフレーム作成
    VideoFrame frame;
    frame.set_width(1920);
    frame.set_height(1080);
    frame.set_data("dummy video data");  // 実際のデータに置き換えてください

    sendVideoData(frame, "127.0.0.1", 5000);

    return 0;
}
