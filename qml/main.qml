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
                        stack.push(setting_service)
                    }
                }
            }
        }
    }

    Component {
        id: setting_service
                Loader {
            source: "qrc:/qml/setting_service.qml"
        }
    }

    Component {
        id: setting_network
                Loader {
            source: "qrc:/qml/setting_network.qml"
        }
    }

    Component {
        id: setting_video
                Loader {
            source: "qrc:/qml/setting_video.qml"
        }
    }

    Component {
        id: setting_audio
                Loader {
            source: "qrc:/qml/setting_audio.qml"
        }
    }

    Component {
        id: setting_telemetry
                Loader {
            source: "qrc:/qml/setting_telemetry.qml"
        }
    }

}

