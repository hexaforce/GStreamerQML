import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import jp.fpv.Q_Telemetry 1.0

Item {
    focus: true
    width: 960
    height: 540

    Q_Telemetry {
        id: q_Telemetry
    }

    Row {
        Column {
            Text {
                text: "This is Telemetry"
            }
            Button {
                text: "Go Back"
                Material.foreground: Material.Primary
                onClicked: stack.pop(stack.find(function(item) {
                    return item.name === "q_telemetry"
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
                enabled: false
                opacity: enabled ? 1.0 : 0.75
            }
        }
        Item {
            visible: true
            width: 460
            height: 340
            ColumnLayout {
                anchors.fill: parent
                spacing: 1
                property var currentItem: null
                Repeater {
                    model: [
                        { title: "Panel 1", color: "orange" },
                        { title: "Panel 2", color: "lightgreen" },
                        { title: "Panel 3", color: "lightblue" },
                        { title: "Panel 4", color: "yellow" }
                    ]
                    delegate: AccordionSection {
                        required property var modelData
                        title: modelData.title
                        contentItem: Rectangle {
                            color: modelData.color
                            anchors.fill: parent
                        }
                    }
                }
                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
    }
}
