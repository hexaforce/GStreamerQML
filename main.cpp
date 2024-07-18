#include "pipeline_manager.h"
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
    qmlRegisterType<Q_Network>("jp.fpv.Q_Network", 1, 0, "Q_Network");
    qmlRegisterType<Q_Video>("jp.fpv.Q_Video", 1, 0, "Q_Video");
    qmlRegisterType<Q_Video>("jp.fpv.Q_Audio", 1, 0, "Q_Audio");
    qmlRegisterType<Q_Video>("jp.fpv.Q_Telemetry", 1, 0, "Q_Telemetry");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QQuickWindow *rootObject = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
    QQuickItem *videoItem = rootObject->findChild<QQuickItem *>("videoItem");
    // QObject *deviceMonitor = rootObject->findChild<QObject *>("deviceMonitor");

    g_object_set(pipelineManager->sink(), "widget", videoItem, NULL);
    rootObject->scheduleRenderJob(pipelineManager, QQuickWindow::BeforeSynchronizingStage);

    ret = app.exec();

    gst_element_set_state(pipelineManager->pipeline(), GST_STATE_NULL);
    gst_object_unref(pipelineManager->pipeline());
  }

  gst_deinit();

  return ret;
}
