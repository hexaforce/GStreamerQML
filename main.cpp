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

#include "q_video.h"
#include "q_audio.h"
#include "q_telemetry.h"
#include "q_network.h"

#include <QtDebug>

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s\n", localMsg.constData());
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s\n", localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s\n", localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s\n", localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\n", localMsg.constData());
        abort();
    }
}

// gst-launch-1.0 -e v4l2src device=/dev/video0 ! 'video/x-h264,width=960,height=540,framerate=30/1' ! h264parse ! rtph264pay ! udpsink host=127.0.0.1 port=5000
int main(int argc, char *argv[])
{
    qInstallMessageHandler(customMessageHandler);

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
    qmlRegisterType<Q_Video>("jp.fpv.Q_Video", 1, 0, "Q_Video");
    qmlRegisterType<Q_Video>("jp.fpv.Q_Audio", 1, 0, "Q_Audio");
    qmlRegisterType<Q_Video>("jp.fpv.Q_Telemetry", 1, 0, "Q_Telemetry");

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
