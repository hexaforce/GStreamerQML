#include "pipeline_thread.h"
#include "process_runner.h"
#include "q_network.h"
#include "udp_receiver.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <gst/gst.h>
#include "q_video.h"
#include "q_audio.h"
#include "q_telemetry.h"
#include "device_monitor.h"

enum class CodecType
{
  H264,
  H265
};
enum class VendorType
{
  Libav,
  Nvidia
};

// gst-launch-1.0 -e udpsrc port=5000 ! application/x-rtp, encoding-name=H264 ! rtph264depay ! h264parse ! nvh264sldec ! nvvideoconvert ! autovideosink sync=false
// gst-launch-1.0 -e udpsrc port=5000 ! application/x-rtp, encoding-name=H265 ! rtph265depay ! h265parse ! nvh265sldec ! nvvideoconvert ! autovideosink sync=false
void setupLocalCapturePipeline(GstElement *pipeline, GstElement *sink)
{
  GstElement *src = gst_element_factory_make("v4l2src", NULL);
  GstElement *videoconvert = gst_element_factory_make("videoconvert", NULL);
  GstElement *glupload = gst_element_factory_make("glupload", NULL);

  gst_bin_add_many(GST_BIN(pipeline), src, videoconvert, glupload, sink, NULL);
  gst_element_link_many(src, videoconvert, glupload, sink, NULL);
}

// gst-launch-1.0 -e udpsrc port=5000 caps="application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)JPEG, payload=(int)26" ! rtpjpegdepay ! avdec_mjpeg ! videoconvert ! autovideosink
// gst-launch-1.0 -e udpsrc port=5000 caps="application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)JPEG, payload=(int)26" ! rtpjpegdepay ! jpegdec ! videoconvert ! autovideosink
void setupJpegReceivePipeline_SoftwareDecoding(GstElement *pipeline, GstElement *sink, gint port, VendorType vendorType)
{
  GstElement *src = gst_element_factory_make("udpsrc", NULL);
  GstElement *rtpdepay = gst_element_factory_make("rtpjpegdepay", NULL);
  GstElement *jpegparse = gst_element_factory_make("jpegparse", NULL);
  GstElement *decoder = gst_element_factory_make(vendorType == VendorType::Libav ? "avdec_mjpeg" : "jpegdec", NULL);
  GstElement *videoconvert = gst_element_factory_make("videoconvert", NULL);
  GstElement *glupload = gst_element_factory_make("glupload", NULL);

  g_object_set(src, "port", port, NULL);
  GstCaps *caps = gst_caps_new_simple("application/x-rtp", "media", G_TYPE_STRING, "video", "encoding-name", G_TYPE_STRING, "JPEG", "payload", G_TYPE_INT, 26, NULL);
  g_object_set(src, "caps", caps, NULL);
  gst_caps_unref(caps);

  gst_bin_add_many(GST_BIN(pipeline), src, rtpdepay, jpegparse, decoder, videoconvert, glupload, sink, NULL);
  gst_element_link_many(src, rtpdepay, jpegparse, decoder, videoconvert, glupload, sink, NULL);
}

// gst-launch-1.0 -e udpsrc port=5000 ! application/x-rtp, encoding-name=H264 ! rtph264depay ! h264parse ! nvv4l2decoder ! nvvideoconvert ! autovideosink sync=false
// gst-launch-1.0 -e udpsrc port=5000 ! application/x-rtp, encoding-name=H265 ! rtph265depay ! h265parse ! nvv4l2decoder ! nvvideoconvert ! autovideosink sync=false
void setupH26xReceivePipeline_HardwareDecoding(GstElement *pipeline, GstElement *sink, gint port, CodecType codecType)
{
  GstElement *src = gst_element_factory_make("udpsrc", NULL);
  GstElement *rtpdepay = nullptr;
  GstElement *parse = nullptr;
  GstElement *decoder = gst_element_factory_make("nvv4l2decoder", NULL);
  GstElement *videoconvert = gst_element_factory_make("nvvideoconvert", NULL);

  g_object_set(src, "port", port, NULL);

  if (codecType == CodecType::H264)
  {
    rtpdepay = gst_element_factory_make("rtph264depay", NULL);
    parse = gst_element_factory_make("h264parse", NULL);
  }
  else if (codecType == CodecType::H265)
  {
    rtpdepay = gst_element_factory_make("rtph265depay", NULL);
    parse = gst_element_factory_make("h265parse", NULL);
  }
  else
  {
    g_printerr("Unsupported codec type.\n");
    return;
  }

  GstCaps *caps = gst_caps_new_simple("application/x-rtp", "media", G_TYPE_STRING, "video", "encoding-name", G_TYPE_STRING, (codecType == CodecType::H264) ? "H264" : "H265", "payload", G_TYPE_INT, 96, NULL);
  g_object_set(src, "caps", caps, NULL);
  gst_caps_unref(caps);

  gst_bin_add_many(GST_BIN(pipeline), src, rtpdepay, parse, decoder, videoconvert, sink, NULL);

  if (!gst_element_link_many(src, rtpdepay, parse, decoder, videoconvert, sink, NULL))
  {
    g_printerr("Failed to link elements.\n");
    return;
  }
}

// gst-launch-1.0 -e udpsrc port=5000 ! application/x-rtp, encoding-name=H264 ! rtph264depay ! h264parse ! avdec_h264 ! videoconvert ! autovideosink
// gst-launch-1.0 -e udpsrc port=5000 ! application/x-rtp, encoding-name=H265 ! rtph265depay ! h265parse ! avdec_h265 ! videoconvert ! autovideosink
// gst-launch-1.0 -e udpsrc port=5000 ! application/x-rtp, encoding-name=H264 ! rtph264depay ! h264parse ! nvh264dec ! videoconvert ! autovideosink
// gst-launch-1.0 -e udpsrc port=5000 ! application/x-rtp, encoding-name=H265 ! rtph265depay ! h265parse ! nvh265dec ! videoconvert ! autovideosink
void setupH26xReceivePipeline_SoftwareDecoding(GstElement *pipeline, GstElement *sink, gint port, CodecType codecType, VendorType vendorType)
{
  GstElement *src = gst_element_factory_make("udpsrc", NULL);
  GstElement *rtpdepay = nullptr;
  GstElement *parse = nullptr;
  GstElement *decoder = nullptr;
  GstElement *videoconvert = gst_element_factory_make("videoconvert", NULL);
  GstElement *glupload = gst_element_factory_make("glupload", NULL);

  g_object_set(src, "port", port, NULL);

  if (codecType == CodecType::H264)
  {
    rtpdepay = gst_element_factory_make("rtph264depay", NULL);
    parse = gst_element_factory_make("h264parse", NULL);
    decoder = gst_element_factory_make(vendorType == VendorType::Libav ? "avdec_h264" : "nvh264dec", NULL);
  }
  else if (codecType == CodecType::H265)
  {
    rtpdepay = gst_element_factory_make("rtph265depay", NULL);
    parse = gst_element_factory_make("h265parse", NULL);
    decoder = gst_element_factory_make(vendorType == VendorType::Libav ? "avdec_h265" : "nvh265dec", NULL);
  }
  else
  {
    g_printerr("Unsupported codec type.\n");
    return;
  }

  GstCaps *caps = gst_caps_new_simple("application/x-rtp", "media", G_TYPE_STRING, "video", "encoding-name", G_TYPE_STRING, (codecType == CodecType::H264) ? "H264" : "H265", "payload", G_TYPE_INT, 96, NULL);
  g_object_set(src, "caps", caps, NULL);
  gst_caps_unref(caps);

  gst_bin_add_many(GST_BIN(pipeline), src, rtpdepay, parse, decoder, videoconvert, glupload, sink, NULL);
  gst_element_link_many(src, rtpdepay, parse, decoder, videoconvert, glupload, sink, NULL);
}

int main(int argc, char *argv[])
{
  int ret;

  gst_init(&argc, &argv);

  {
    QGuiApplication app(argc, argv);

    GstElement *pipeline = gst_pipeline_new(NULL);
    GstElement *sink = gst_element_factory_make("qmlglsink", NULL);

    gint port = 5000;
    // setupLocalCapturePipeline(pipeline, sink);
    setupH26xReceivePipeline_SoftwareDecoding(pipeline, sink, port, CodecType::H264, VendorType::Libav);
    // setupH265ReceivePipeline(pipeline, sink, port);
    // setupJpegReceivePipeline(pipeline, sink, port);

    qmlRegisterType<DeviceMonitor>("jp.fpv.DeviceMonitor", 1, 0, "DeviceMonitor");
    qmlRegisterType<ProcessRunner>("jp.fpv.processrunner", 1, 0, "ProcessRunner");
    qmlRegisterType<UdpReceiver>("jp.fpv.UdpReceiver", 1, 0, "UdpReceiver");
    qmlRegisterType<Q_Network>("jp.fpv.Q_Network", 1, 0, "Q_Network");
    qmlRegisterType<Q_Video>("jp.fpv.Q_Video", 1, 0, "Q_Video");
    qmlRegisterType<Q_Video>("jp.fpv.Q_Audio", 1, 0, "Q_Audio");
    qmlRegisterType<Q_Video>("jp.fpv.Q_Telemetry", 1, 0, "Q_Telemetry");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QQuickItem *videoItem;
    QQuickWindow *rootObject;

    rootObject = static_cast<QQuickWindow *>(engine.rootObjects().first());
    videoItem = rootObject->findChild<QQuickItem *>("videoItem");
    g_object_set(sink, "widget", videoItem, NULL);

    rootObject->scheduleRenderJob(new PipelineThread(pipeline), QQuickWindow::BeforeSynchronizingStage);

    ret = app.exec();

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
  }

  gst_deinit();

  return ret;
}
