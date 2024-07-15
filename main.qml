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
    width: 640
    height: 480
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

    Image {
        layer.enabled: true
        x:10
        y:10
        source: "icons/online-streaming-icon.svg"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                // stack.push(receiverWifi)
                processRunner.runCommand("v4l2-ctl", ["--list-devices"])
                console.log(processRunner.result)
            }
        }
    }

    Item {
        anchors.fill: parent
        // Button {
        //     text: "List Devices"
        //     onClicked: {
        //         processRunner.runCommand("v4l2-ctl", ["--list-devices"])
        //         console.log(processRunner.result)
        //     }
        // }
        GstGLVideoItem {
            id: video
            objectName: "videoItem"
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
        }

        // Rectangle {
        //     color: Qt.rgba(1, 1, 1, 0.7)
        //     border.width: 1
        //     border.color: "white"
        //     anchors.bottom: video.bottom
        //     anchors.bottomMargin: 15
        //     anchors.horizontalCenter: parent.horizontalCenter
        //     width : parent.width - 30
        //     height: parent.height - 30
        //     radius: 8

        //     MouseArea {
        //         id: mousearea
        //         anchors.fill: parent
        //         hoverEnabled: true
        //         onEntered: {
        //             // parent.opacity = 1.0
        //             processRunner.runCommand("v4l2-ctl", ["--list-devices"])
        //             console.log(processRunner.result)
        //             // hidetimer.start()
        //         }
        //     }

        //     // Timer {
        //     //     id: hidetimer
        //     //     interval: 5000
        //     //     onTriggered: {
        //     //         parent.opacity = 0.0
        //     //         stop()
        //     //     }
        //     // }
        // }
    }

}
