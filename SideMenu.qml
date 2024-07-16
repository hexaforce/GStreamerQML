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
            text: "Go Network"
            Layout.fillWidth: true
            onClicked: stack.push(q_network)
            enabled: current !== "q_network"
            opacity: enabled ? 1.0 : 0.75
        }

        Button {
            text: "Go Video"
            Layout.fillWidth: true
            onClicked: stack.push(q_video)
            enabled: current !== "q_video"
            opacity: enabled ? 1.0 : 0.75
        }

        Button {
            text: "Go Audio"
            Layout.fillWidth: true
            onClicked: stack.push(q_audio)
            enabled: current !== "q_audio"
            opacity: enabled ? 1.0 : 0.75
        }

        Button {
            text: "Go Telemetry"
            Layout.fillWidth: true
            onClicked: stack.push(q_telemetry)
            enabled: current !== "q_telemetry"
            opacity: enabled ? 1.0 : 0.75
        }
    }
}
