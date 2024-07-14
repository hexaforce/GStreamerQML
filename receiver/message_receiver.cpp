#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include "../message/video_data.pb.h"  // protocで生成されたヘッダー

void receiveVideoData(gint port) {
    // GStreamer初期化
    gst_init(NULL, NULL);

    // パイプライン作成
    GstElement *pipeline = gst_pipeline_new("video-receive-pipeline");
    GstElement *tcpserversrc = gst_element_factory_make("tcpserversrc", "tcp-src");
    GstElement *appsink = gst_element_factory_make("appsink", "video-sink");

    // パイプラインに要素を追加
    gst_bin_add_many(GST_BIN(pipeline), tcpserversrc, appsink, NULL);

    // 要素をリンク
    if (!gst_element_link(tcpserversrc, appsink)) {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return;
    }

    // tcpserversrcのプロパティ設定
    g_object_set(tcpserversrc, "port", port, NULL);

    // パイプラインを再生状態に設定
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // メインループ
    // GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    // g_main_loop_run(loop);

    // データ受信処理
    GstSample *sample = gst_app_sink_pull_sample(GST_APP_SINK(appsink));
    GstBuffer *buffer = gst_sample_get_buffer(sample);
    GstMapInfo map;
    gst_buffer_map(buffer, &map, GST_MAP_READ);

    // プロトコルバッファをデシリアライズ
    VideoFrame frame;
    frame.ParseFromArray(map.data, map.size);

    // フレームの内容を表示
    g_print("Received frame: width=%d, height=%d\n", frame.width(), frame.height());

    // クリーンアップ
    gst_buffer_unmap(buffer, &map);
    gst_sample_unref(sample);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
}

int main(int argc, char *argv[]) {
    receiveVideoData(5000);

    return 0;
}
