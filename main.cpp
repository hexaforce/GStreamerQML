#include "pipeline_manager.h"

#include "common_device_monitor.h"
#include "process_runner.h"
#include "udp_receiver.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <gst/gst.h>

#include "q_network.h"
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

    // PipelineManager *pipelineManager = new PipelineManager();

    qmlRegisterType<CommonDeviceMonitor>("jp.fpv.CommonDeviceMonitor", 1, 0, "CommonDeviceMonitor");
    qmlRegisterType<ProcessRunner>("jp.fpv.ProcessRunner", 1, 0, "ProcessRunner");
    qmlRegisterType<UdpReceiver>("jp.fpv.UdpReceiver", 1, 0, "UdpReceiver");
    // qmlRegisterType<PipelineManager>("jp.fpv.PipelineManager", 1, 0, "PipelineManager");
    
    qmlRegisterType<Q_Network>("jp.fpv.Q_Network", 1, 0, "Q_Network");

    QQmlApplicationEngine engine;
    PipelineManager pipelineManager;
    engine.rootContext()->setContextProperty("pipelineManager", &pipelineManager);
    // ProcessRunner processRunner;
    // engine.rootContext()->setContextProperty("processRunner", &processRunner);
    

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QQuickWindow *rootObject = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
    QQuickItem *videoItem = rootObject->findChild<QQuickItem *>("videoItem");
    g_object_set(pipelineManager.sink(), "widget", videoItem, NULL);

    ret = app.exec();

    gst_element_set_state(pipelineManager.pipeline(), GST_STATE_NULL);
    gst_object_unref(pipelineManager.pipeline());
  }

  gst_deinit();

  return ret;
}
