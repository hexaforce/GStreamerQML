import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import jp.fpv.Q_Telemetry 1.0

Item {
    focus: true

    Q_Telemetry {
        id: q_Telemetry
    }
    Row {
        SideMenu {
            id: sideMenu
            current: "q_telemetry"
        }

        Item {
            visible: true
            width: window.width - sideMenu.width ; height: window.height

            ColumnLayout {
                anchors.fill: parent
                spacing: 1

                property var currentItem: null
                Text {
                    id: name1
                    text: qsTr("text")
                }

                Repeater {
                    model: [
                        { title: "Panel1 1", color: "red" },
                        { title: "Panel1 2", color: "green" },
                        { title: "Panel1 3", color: "blue" },
                        { title: "Panel1 4", color: "yellow" }
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

                Text {
                    id: name2
                    text: qsTr("text")
                }

                Repeater {
                    model: [
                        { title: "Panel2 1", color: "orange" },
                        { title: "Panel2 2", color: "lightgreen" },
                        { title: "Panel2 3", color: "lightblue" },
                        { title: "Panel2 4", color: "yellow" }
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
