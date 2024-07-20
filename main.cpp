#include "pipeline_manager.h"
#include "pipeline_controller.h"

#include "device_monitor.h"
#include "process_runner.h"
#include "udp_receiver.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <gst/gst.h>

#include "q_network.h"

#include <QtDebug>

// gst-launch-1.0 -e v4l2src device=/dev/video0 ! 'video/x-h264,width=960,height=540,framerate=30/1' ! h264parse ! rtph264pay ! udpsink host=127.0.0.1 port=5000
int main(int argc, char *argv[])
{

  int ret;

  gst_init(&argc, &argv);

  {
    QGuiApplication app(argc, argv);

    PipelineManager *pipelineManager = new PipelineManager();

    qmlRegisterType<DeviceMonitor>("jp.fpv.DeviceMonitor", 1, 0, "DeviceMonitor");
    qmlRegisterType<ProcessRunner>("jp.fpv.processrunner", 1, 0, "ProcessRunner");
    qmlRegisterType<UdpReceiver>("jp.fpv.UdpReceiver", 1, 0, "UdpReceiver");
    qmlRegisterType<UdpReceiver>("jp.fpv.UdpReceiver", 1, 0, "UdpReceiver");
    qmlRegisterType<PipelineController>("jp.fpv.PipelineController", 1, 0, "PipelineController");
    
    qmlRegisterType<Q_Network>("jp.fpv.Q_Network", 1, 0, "Q_Network");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QQuickWindow *rootObject = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
    QQuickItem *videoItem = rootObject->findChild<QQuickItem *>("videoItem");
    PipelineController *pipelineController = rootObject->findChild<PipelineController *>("pipelineController");
    pipelineController->setPipelineManager(pipelineManager);

    g_object_set(pipelineManager->sink(), "widget", videoItem, NULL);
    rootObject->scheduleRenderJob(pipelineManager, QQuickWindow::BeforeSynchronizingStage);

    ret = app.exec();

    gst_element_set_state(pipelineManager->pipeline(), GST_STATE_NULL);
    gst_object_unref(pipelineManager->pipeline());
  }

  gst_deinit();

  return ret;
}
