import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import org.freedesktop.gstreamer.GLVideoItem 1.0

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
    }

    Component {
        id: mainView

        Item {
            anchors.fill: parent

            Image {
                layer.enabled: true
                x:10
                y:10
                source: "icons/online-streaming-icon.svg"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        processRunner.runCommand("v4l2-ctl", ["--list-devices"])
                        console.log(processRunner.result)
                        stack.push(q_network)
                    }
                }
            }
        }
    }

    Component {
        id: q_network
        Q_network { }
    }

    Component {
        id: q_video
        Q_video { }
    }

    Component {
        id: q_audio
        Q_audio { }
    }

    Component {
        id: q_telemetry
        Q_telemetry { }
    }
}
