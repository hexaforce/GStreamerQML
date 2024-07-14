import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

import org.freedesktop.gstreamer.GLVideoItem 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    x: 30
    y: 30
    color: "black"

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

    // Instantiate ProtobufHandler
    property ProtobufHandler protobufHandler: ProtobufHandler {}

    Text {
        id: receivedMessage
        anchors.centerIn: parent
        text: "Received Message: "
    }

    // Connect signal from C++ to update UI
    Connections {
        target: protobufHandler
        onDataReceived: {
            receivedMessage.text = "Received Message: " + message
        }
    }

    Component.onCompleted: {
        // Simulate receiving protobuf data
        var data = new QByteArray();
        data.append(" \x0A\x0D\x03\x68\x65\x6C\x6C\x6F\x07\x77\x6F\x72\x6C\x64", 14);
        protobufHandler.receiveData(data);
    }
        }
    }
}
