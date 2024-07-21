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
                Loader {
            source: "qrc:/qml/service_setting.qml"
        }
    }

    Component {
        id: q_network
                Loader {
            source: "qrc:/qml/network_setting.qml"
        }
    }

    Component {
        id: q_video
                Loader {
            source: "qrc:/qml/video_setting.qml"
        }
    }

    Component {
        id: q_audio
                Loader {
            source: "qrc:/qml/audio_setting.qml"
        }
    }

    Component {
        id: q_telemetry
                Loader {
            source: "qrc:/qml/telemetry_setting.qml"
        }
    }

}

