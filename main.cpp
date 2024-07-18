#include "pipeline_controller.h"
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
#include "receive_pipelines.h"
#include "pipeline_controller.h"

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

    rootObject->scheduleRenderJob(new PipelineController(pipeline), QQuickWindow::BeforeSynchronizingStage);

    ret = app.exec();

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
  }

  gst_deinit();

  return ret;
}
