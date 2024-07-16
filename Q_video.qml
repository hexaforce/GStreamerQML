import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import jp.fpv.Q_Video 1.0 


Item {
    focus: true

    Q_Video {
        id: q_Video
    }

    Row {

        SideMenu {
            current: "q_video"
        }

        Item {
            visible: true
            width: 600
            height: 500
            Button {
                text: "List Video Devices"
                onClicked: {
                    console.log(deviceMonitor.listDevices("Video"))
                }
            }
            ColumnLayout {
                anchors.fill: parent
                spacing: 1
                property var currentItem: null
                Repeater {
                    model: JSON.parse(deviceMonitor.listDevices("Video"))
                    delegate: AccordionSection {
                        required property var modelData
                        title: modelData.Device
                        contentItem: Rectangle {
                            color: "lightgreen"
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
