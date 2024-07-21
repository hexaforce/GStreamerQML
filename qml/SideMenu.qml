import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Column {
    property string current: ""

    Text {
        text: "This is " + current
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
            onClicked: stack.push(setting_service)
            enabled: current !== "setting_service"
            opacity: enabled ? 1.0 : 0.75
        }

        Button {
            text: "Go Network"
            Layout.fillWidth: true
            onClicked: stack.push(setting_network)
            enabled: current !== "setting_network"
            opacity: enabled ? 1.0 : 0.75
        }

        Button {
            text: "Go Video"
            Layout.fillWidth: true
            onClicked: stack.push(setting_video)
            enabled: current !== "setting_video"
            opacity: enabled ? 1.0 : 0.75
        }

        Button {
            text: "Go Audio"
            Layout.fillWidth: true
            onClicked: stack.push(setting_audio)
            enabled: current !== "setting_audio"
            opacity: enabled ? 1.0 : 0.75
        }

        Button {
            text: "Go Telemetry"
            Layout.fillWidth: true
            onClicked: stack.push(setting_telemetry)
            enabled: current !== "setting_telemetry"
            opacity: enabled ? 1.0 : 0.75
        }
    }
}
