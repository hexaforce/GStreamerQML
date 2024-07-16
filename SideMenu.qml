import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

Column {
    property string current: ""

    Text {
        text: "This is "+current
    }
    Button {
        text: "Go Back"
        Material.foreground: Material.Primary
        onClicked: stack.pop(stack.find(function(item) {
            return item.name === current
        }))
    }
    Button {
        text: "Go Network"
        Material.foreground: Material.Primary
        onClicked: stack.push(q_network)
        enabled: current !== "q_network"
        opacity: enabled ? 1.0 : 0.75
    }
    Button {
        text: "Go Video"
        Material.foreground: Material.Primary
        onClicked: stack.push(q_video)
        enabled: current !== "q_video"
        opacity: enabled ? 1.0 : 0.75
    }
    Button {
        text: "Go Audio"
        Material.foreground: Material.Primary
        onClicked: stack.push(q_audio)
        enabled: current !== "q_audio"
        opacity: enabled ? 1.0 : 0.75
    }
    Button {
        text: "Go Telemetry"
        Material.foreground: Material.Primary
        onClicked: stack.push(q_telemetry)
        enabled: current !== "q_telemetry"
        opacity: enabled ? 1.0 : 0.75
    }
}
