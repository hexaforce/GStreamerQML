import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Column {
    property string current: ""

    Text {
        text: "menu " + current
    }

    GridLayout {
        columns: 1

        Button {
            text: "Go Back"
            Layout.fillWidth: true
            onClicked: stack.pop(stack.find(function(item) {
                return item.name === current
            }))
        }

        Button {
            text: "Go Service"
            Layout.fillWidth: true
            onClicked: stack.push(q_service)
            enabled: current !== "q_service"
            opacity: enabled ? 1.0 : 0.75
        }

        Button {
            text: "Go Network"
            Layout.fillWidth: true
            onClicked: stack.push(q_network)
            enabled: current !== "q_network"
            opacity: enabled ? 1.0 : 0.75
        }

        Button {
            text: "Go Video"
            Layout.fillWidth: true
            onClicked: stack.push(videosetting)
            enabled: current !== "videosetting"
            opacity: enabled ? 1.0 : 0.75
        }

        Button {
            text: "Go Audio"
            Layout.fillWidth: true
            onClicked: stack.push(audiosetting)
            enabled: current !== "audiosetting"
            opacity: enabled ? 1.0 : 0.75
        }

        Button {
            text: "Go Telemetry"
            Layout.fillWidth: true
            onClicked: stack.push(telemetrysetting)
            enabled: current !== "telemetrysetting"
            opacity: enabled ? 1.0 : 0.75
        }
    }
}
