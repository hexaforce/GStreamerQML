#include <QCoreApplication>
#include <gst/gst.h>
#include <QTimer>
#include "transmit_pipelines.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    gst_init(&argc, &argv);

    GstElement *video_pipeline = gst_pipeline_new("video_pipeline");
    GstElement *audio_pipeline = gst_pipeline_new("audio_pipeline");

    const char *video_device = "/dev/video0";
    const char *audio_device = "default";
    const char *host = "127.0.0.1";
    int video_port = 5000;
    int audio_port = 5001;

    // GstStateChangeReturn ret_video = setupVideoTransmitPipeline(video_pipeline, video_device, host, video_port, CodecType::H264);
    GstStateChangeReturn ret_video = setupH26xTransmitPipeline(video_pipeline, video_device, host, video_port, CodecType::H264);
    GstStateChangeReturn ret_audio = setupAudioTransmitPipeline(audio_pipeline, audio_device, host, audio_port);

    if (ret_video == GST_STATE_CHANGE_FAILURE || ret_audio == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Failed to start one or both pipelines.\n");
        return -1;
    }
    GstBus     *bus = gst_element_get_bus(video_pipeline);
    GstMessage *msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_EOS);

    // Qtのタイマーを使って定期的にGStreamerのバスメッセージを処理
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        GstBus *video_bus = gst_element_get_bus(video_pipeline);
         GstMessage *video_msg = gst_bus_timed_pop_filtered(video_bus, GST_CLOCK_TIME_NONE, static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
        if (video_msg) {
            // エラーメッセージの処理
            gst_message_unref(video_msg);
        }
        gst_object_unref(video_bus);

        GstBus *audio_bus = gst_element_get_bus(audio_pipeline);
        GstMessage *audio_msg = gst_bus_timed_pop_filtered(video_bus, GST_CLOCK_TIME_NONE, static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
        if (audio_msg) {
            // エラーメッセージの処理
            gst_message_unref(audio_msg);
        }
        gst_object_unref(audio_bus);
    });
    timer.start(100); // 100ミリ秒ごとにバスメッセージをチェック

    return a.exec();
}
