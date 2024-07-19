import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import org.freedesktop.gstreamer.GLVideoItem 1.0

// import jp.fpv.CommonDeviceMonitor 1.0
import jp.fpv.CommonProcessRunner 1.0
import jp.fpv.UdpReceiver 1.0
// import jp.fpv.CommonNetworkService 1.0

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

    // CommonNetworkService {
    //     id: commonNetworkService
    // }

    // CommonDeviceMonitor {
    //     id: commonDeviceMonitor
    // }

    CommonProcessRunner {
        id: commonProcessRunner
        onResultChanged: {
            console.log("Command Result:", commonProcessRunner.result)
        }
        onErrorChanged: {
            console.log("Command Error:", commonProcessRunner.error)
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
        // console.log( commonPipelineManager.get_pipeline_state())
        // console.log(  commonPipelineManager.get_pipeline_parameters())
        // id: commonProcessRunner
        // commonProcessRunner.onResultChanged=()=>{
        //     console.log("Command Result:", commonProcessRunner.result)
        // }
        // commonProcessRunner.onErrorChanged=()=>{
        //     console.log("Command Error:", commonProcessRunner.error)
        // }
        // // onErrorChanged: {
        // //     console.log("Command Error:", commonProcessRunner.error)
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
                        // commonProcessRunner.runCommand("v4l2-ctl", ["--list-devices"])
                        // console.log(commonProcessRunner.result)
                        stack.push(servicesetting)
                    }
                }
            }
        }
    }

    Component {
        id: servicesetting
        Loader {
            source: "qrc:/qml/ServiceSetting.qml"
        }
    }

    Component {
        id: networksetting
        Loader {
            source: "qrc:/qml/NetworkSetting.qml"
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
