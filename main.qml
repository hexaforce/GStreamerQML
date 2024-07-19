import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import org.freedesktop.gstreamer.GLVideoItem 1.0

import jp.fpv.DeviceMonitor 1.0
import jp.fpv.ProcessRunner 1.0
import jp.fpv.UdpReceiver 1.0
import jp.fpv.Q_Network 1.0

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

    Q_Network {
        id: q_Network
    }

    DeviceMonitor {
        id: deviceMonitor
    }

    ProcessRunner {
        id: processRunner
        onResultChanged: {
            console.log("Command Result:", processRunner.result)
        }
        onErrorChanged: {
            console.log("Command Error:", processRunner.error)
        }
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
        // id: processRunner
        // processRunner.onResultChanged=()=>{
        //     console.log("Command Result:", processRunner.result)
        // }
        // processRunner.onErrorChanged=()=>{
        //     console.log("Command Error:", processRunner.error)
        // }
        // // onErrorChanged: {
        // //     console.log("Command Error:", processRunner.error)
        // // }
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
                source: "qml/icons/online-streaming-icon.svg"
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
            source: "qrc:/qml/Q_service.qml"
        }
    }

    Component {
        id: q_network
        Loader {
            source: "qrc:/qml/Q_network.qml"
        }
    }

    Component {
        id: videosetting
        Loader {
            source: "qrc:/qml/VideoSetting.qml"
        }
    }

    Component {
        id: audiosetting
        Loader {
            source: "qrc:/qml/AudioSetting.qml"
        }
    }

    Component {
        id: telemetrysetting
        Loader {
            source: "qrc:/qml/TelemetrySetting.qml"
        }

    }

}
