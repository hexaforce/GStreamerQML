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

    StackView {
        id: stack
        initialItem: mainView
        anchors.fill: parent
    }

    Item {
        id: mainView
        anchors.fill: parent
        GstGLVideoItem {
            id: video
            objectName: "videoItem"
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
        }
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
                    stack.push(page1)
                }
            }
        }
    }

    Component {
        id: page1
        Item {
            focus: true
            width: 640
            height: 480
            Row{
                Column {
                    Text {
                        text: "This is page1"
                    }
                    Button {
                        text: "Go Back"
                        Material.foreground: Material.Primary
                        onClicked: stack.pop(stack.find(function(item) {
                            return item.name === "page1"
                        }))
                    }
                    Button {
                        text: "Go page1"
                        Material.foreground: Material.Primary
                        onClicked: stack.push(page1)
                        enabled: false
                        opacity: enabled ? 1.0 : 0.75
                    }
                    Button {
                        text: "Go page2"
                        Material.foreground: Material.Primary
                        onClicked: stack.push(page2)
                    }
                }
                Column {
                    Rectangle {
                        width: 400
                        height: 300
                        color: "red"
                    }
                }
            }
        }
    }
    Component {
        id: page2
        Item {
            focus: true
            width: 640
            height: 480
            Row{
                Column {
                    Text {
                        text: "This is page2"
                    }
                    Button {
                        text: "Go Back"
                        Material.foreground: Material.Primary
                        onClicked: stack.pop(stack.find(function(item) {
                            return item.name === "page2"
                        }))
                    }
                    Button {
                        text: "Go page1"
                        Material.foreground: Material.Primary
                        onClicked: stack.push(page1)
                    }
                    Button {
                        text: "Go page2"
                        Material.foreground: Material.Primary
                        onClicked: stack.push(page2)
                        enabled: false
                        opacity: enabled ? 1.0 : 0.75
                    }
                }
                Column {
                    Rectangle {
                        width: 400
                        height: 300
                        color: "blue"
                    }

                }
            }
        }
    }
}
