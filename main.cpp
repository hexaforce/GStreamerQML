#include "common_pipeline_manager.h"

#include "common_device_monitor.h"
#include "common_process_runner.h"
#include "udp_receiver.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <gst/gst.h>

#include "common_network_service.h"
#include <QQuickItem>
#include <QDebug>

#include <QQmlContext>


// gst-launch-1.0 -e v4l2src device=/dev/video0 ! 'video/x-h264,width=960,height=540,framerate=30/1' ! h264parse ! rtph264pay ! udpsink host=127.0.0.1 port=5000
int main(int argc, char *argv[])
{

  int ret;

  gst_init(&argc, &argv);

  {
    QGuiApplication app(argc, argv);

    qmlRegisterType<CommonProcessRunner>("jp.fpv.CommonProcessRunner", 1, 0, "CommonProcessRunner");
    qmlRegisterType<UdpReceiver>("jp.fpv.UdpReceiver", 1, 0, "UdpReceiver");

    QQmlApplicationEngine engine;
    CommonPipelineManager commonPipelineManager;
    engine.rootContext()->setContextProperty("commonPipelineManager", &commonPipelineManager);
    CommonDeviceMonitor commonDeviceMonitor;
    engine.rootContext()->setContextProperty("commonDeviceMonitor", &commonDeviceMonitor);
    CommonNetworkService commonNetworkService;
    engine.rootContext()->setContextProperty("commonNetworkService", &commonNetworkService);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    QQuickWindow *rootObject = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
    QQuickItem *videoItem = rootObject->findChild<QQuickItem *>("videoItem");
    g_object_set(commonPipelineManager.sink(), "widget", videoItem, NULL);

    ret = app.exec();

    gst_element_set_state(commonPipelineManager.pipeline(), GST_STATE_NULL);
    gst_object_unref(commonPipelineManager.pipeline());
  }

  gst_deinit();

  return ret;
}
