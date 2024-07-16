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
        // Column {
        //     Rectangle {
        //         width: 400
        //         height: 300
        //         color: "yellow"
        //     }
        // }

        ColumnLayout {
            anchors.fill: parent
            spacing: 1
            property var currentItem: null
            PanelItem {
                title: "Panel 1"
                Rectangle {
                    color: "orange"
                    anchors.fill: parent
                }
            }
            PanelItem {
                title: "Panel 2"
                Rectangle {
                    color: "lightgreen"
                    anchors.fill: parent
                }
            }
            PanelItem {
                title: "Panel 3"
                Rectangle {
                    color: "lightblue"
                    anchors.fill: parent
                }
            }
            PanelItem {
                title: "Panel 4"
                Rectangle {
                    color: "yellow"
                    anchors.fill: parent
                }
            }
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        // Column {
        //     spacing: 5
        //     AccItem {
        //         Rectangle {
        //             width: 50
        //             height: 50
        //             radius: 50
        //             color: "blue"
        //             anchors.centerIn: parent
        //         }
        //     }
        //     AccItem {
        //         Rectangle {
        //             width: 100
        //             height: 100
        //             radius: 50
        //             color: "yellow"
        //             anchors.centerIn: parent
        //         }
        //     }
        //     AccItem {
        //         Rectangle {
        //             width: 75
        //             height: 75
        //             radius: 50
        //             color: "cyan"
        //             anchors.centerIn: parent
        //         }
        //     }
        // }
    }
}
