#include "common_pipeline_manager.h"
#include "common_network_service.h"
#include "common_device_monitor.h"
#include "common_process_runner.h"
#include "common_telemetry_socket.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <gst/gst.h>

#include <QQuickItem>
#include <QDebug>

#include <QQmlContext>

#include "receive_pipelines.h"


// g++ -o video_streamer video_streamer.cpp `pkg-config --cflags --libs gstreamer-1.0`

// if (argc != 4) {
//   g_printerr("Usage: %s <device> <host> <port>\n", argv[0]);
//   return -1;
// }

// const char *device = argv[1];
// const char *host   = argv[2];
// int         port   = atoi(argv[3]);

#include <gst/gst.h>
#include "transmit_pipelines.h"

int main(int argc, char *argv[]) {

  gst_init(&argc, &argv);

  {
    const char *device = "/dev/video0";
    const char *host   = "127.0.0.1"; // destinationAddress
    int         port   = 5000;        // videoPort

    // Create elements
    GstElement *pipeline = gst_pipeline_new(NULL);

    // GstStateChangeReturn ret = setupJPEGTransmitPipeline(pipeline, device, host, port);
    GstStateChangeReturn ret = setupH26xTransmitPipeline(pipeline, device, host, port, CodecType::H264);
    // GstStateChangeReturn ret_h264 = setupH26xTransmitPipeline_JpegToH26x_SoftwareEncoding(pipeline, device, host, port, CodecType::H264);
    // GstStateChangeReturn ret_h264 = setupH26xTransmitPipeline_RawToH26x_SoftwareEncoding(pipeline, device, host, port, CodecType::H264);

    // GstStateChangeReturn ret = setupH265TransmitPipeline(pipeline, device, host, port);
    if (ret == GST_STATE_CHANGE_FAILURE) {
      g_printerr("Failed to start pipeline.\n");
      return -1;
    }

    // Wait until error or EOS
    GstBus     *bus = gst_element_get_bus(pipeline);
    GstMessage *msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_EOS);

    // Parse message
    if (msg != NULL) {
      GError *err;
      gchar  *debug_info;
      switch (GST_MESSAGE_TYPE(msg)) {
      case GST_MESSAGE_ERROR:
        gst_message_parse_error(msg, &err, &debug_info);
        g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
        g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
        g_clear_error(&err);
        g_free(debug_info);
        break;
      case GST_MESSAGE_EOS:
        g_print("End-Of-Stream reached.\n");
        break;
      default:
        // Should not be reached
        g_printerr("Unexpected message received.\n");
        break;
      }
      gst_message_unref(msg);
    }

    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
  }

  return 0;
}


// // gst-launch-1.0 -e v4l2src device=/dev/video0 ! 'video/x-h264,width=960,height=540,framerate=30/1' ! h264parse ! rtph264pay ! udpsink host=127.0.0.1 port=5000
// int main(int argc, char *argv[])
// {

//   int ret;

//   gst_init(&argc, &argv);

//   {
//     QGuiApplication app(argc, argv);

//     qmlRegisterType<CommonProcessRunner>("jp.fpv.CommonProcessRunner", 1, 0, "CommonProcessRunner");
//     qmlRegisterType<CommonTelemetrySocket>("jp.fpv.CommonTelemetrySocket", 1, 0, "CommonTelemetrySocket");

//     QQmlApplicationEngine engine;
//     CommonPipelineManager commonPipelineManager;
//     setupH26xReceivePipeline_SoftwareDecoding(commonPipelineManager.pipeline(), commonPipelineManager.sink(), 5000, CodecType::H264, VendorType::Libav);
//     engine.rootContext()->setContextProperty("commonPipelineManager", &commonPipelineManager);
//     CommonDeviceMonitor commonDeviceMonitor;
//     engine.rootContext()->setContextProperty("commonDeviceMonitor", &commonDeviceMonitor);
//     CommonNetworkService commonNetworkService;
//     engine.rootContext()->setContextProperty("commonNetworkService", &commonNetworkService);

//     engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

//     QQuickWindow *rootObject = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
//     QQuickItem *videoItem = rootObject->findChild<QQuickItem *>("videoItem");
//     g_object_set(commonPipelineManager.sink(), "widget", videoItem, NULL);

//     ret = app.exec();

//     gst_element_set_state(commonPipelineManager.pipeline(), GST_STATE_NULL);
//     gst_object_unref(commonPipelineManager.pipeline());
//   }

//   gst_deinit();

//   return ret;
// }
