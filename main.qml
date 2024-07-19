import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import org.freedesktop.gstreamer.GLVideoItem 1.0

import jp.fpv.DeviceMonitor 1.0
import jp.fpv.processrunner 1.0
import jp.fpv.UdpReceiver 1.0
import jp.fpv.PipelineManager 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 960
    height: 540
    color: "transparent"
    // flags: Qt.FramelessWindowHint
    x: (Screen.width - width) / 2
    y: (Screen.height - height) / 2

    Material.theme: Material.Dark

    DeviceMonitor {
        id: deviceMonitor
    }

    PipelineManager {
        id: pipelineManager
        objectName: "pipelineManager"
    }

    ProcessRunner {
        id: processRunner
    }

    UdpReceiver {
        id: udpReceiver
        onMessageReceived: {
            console.log("Received message in QML:", message)
        }
    }

    Component.onCompleted: {
        udpReceiver.startListening(5009)
        // console.log( pipelineManager.get_pipeline_state())
        // console.log(  pipelineManager.get_pipeline_parameters())
    }

    Item {
        anchors.fill: parent
        GstGLVideoItem {
            objectName: "videoItem"
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
        }
    }

    StackView {
        id: stack
        initialItem: mainView
        anchors.fill: parent
        pushEnter: Transition {
            id: pushEnter
            ParallelAnimation {
                PropertyAction { property: "x"; value: pushEnter.ViewTransition.item.pos }
                NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 400; easing.type: Easing.OutCubic }
            }
        }
    }

    Component {
        id: mainView

        Item {

            Image {
                layer.enabled: true
                x:10
                y:10
                source: "icons/online-streaming-icon.svg"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        // processRunner.runCommand("v4l2-ctl", ["--list-devices"])
                        // console.log(processRunner.result)
                        stack.push(q_service)
                    }
                }
            }
        }
    }

    Component {
        id: q_service
        Q_service {  }
    }

    Component {
        id: q_network
        Q_network {  }
    }

    Component {
        id: q_video
        Q_video { }
    }

    Component {
        id: q_audio
        Q_audio {  }
    }

    Component {
        id: q_telemetry
        Q_telemetry {  }
    }

}
