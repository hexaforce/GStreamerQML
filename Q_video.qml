import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import jp.fpv.Q_Video 1.0 


Item {
    focus: true
    width: 960
    height: 540

    Q_Video {
        id: q_Video
    }

    Row {
        Column {
            Text {
                text: "This is Video"
            }
            Button {
                text: "Go Back"
                Material.foreground: Material.Primary
                onClicked: stack.pop(stack.find(function(item) {
                    return item.name === "q_video"
                }))
            }
            Button {
                text: "Go Network"
                Material.foreground: Material.Primary
                onClicked: stack.push(q_network)
            }
            Button {
                text: "Go Video"
                Material.foreground: Material.Primary
                onClicked: stack.push(q_video)
                enabled: false
                opacity: enabled ? 1.0 : 0.75
            }
            Button {
                text: "Go Audio"
                Material.foreground: Material.Primary
                onClicked: stack.push(q_audio)
            }
            Button {
                text: "Go Telemetry"
                Material.foreground: Material.Primary
                onClicked: stack.push(q_telemetry)
            }
        }
        Column {
            ComboBox {
                id: cameraComboBox
                width: 240
                // model: q_Video.getVideoDevices()
                onCurrentIndexChanged: {

                }
            }
            Rectangle {
                width: 400
                height: 300
                color: "blue"
            }
        }
    }
}
