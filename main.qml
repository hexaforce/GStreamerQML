import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

import org.freedesktop.gstreamer.GLVideoItem 1.0

import com.example 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    x: 30
    y: 30
    color: "black"
    ProtobufHandler {
        id: protobufHandler // QML内で使う場合はインスタンスを生成する
    }
    Item {
        anchors.fill: parent

        GstGLVideoItem {
            id: video
            objectName: "videoItem"
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
        }

        Rectangle {
            color: Qt.rgba(1, 1, 1, 0.7)
            border.width: 1
            border.color: "white"
            anchors.bottom: video.bottom
            anchors.bottomMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter
            width : parent.width - 30
            height: parent.height - 30
            radius: 8

            MouseArea {
                id: mousearea
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    parent.opacity = 1.0
                    hidetimer.start()
                }
            }

            Timer {
                id: hidetimer
                interval: 5000
                onTriggered: {
                    parent.opacity = 0.0
                    stop()
                }
            }
        }
    }

    Item {
        Text {
            id: receivedMessage
            text: "Waiting for message..."
            anchors.centerIn: parent
        }

        Connections {
            target: protobufHandler
            function onMessageReceived() {
                receivedMessage.text = "Received Message: " + protobufHandler.receivedMessage
            }
        }

        Component.onCompleted: {
            // Simulate receiving protobuf data
            var base64Data = "CgwDaGVsbG8Hd29ybGQ="; // base64-encoded protobuf data
            protobufHandler.receiveData(base64Data);
        }
    }
}
