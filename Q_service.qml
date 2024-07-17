import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

// import jp.fpv.Q_Service 1.0

Item {
    focus: true

    // Q_Service {
    //     id: q_Service
    // }

    property var combinedStatus: null
    // Component.onCompleted: {
    //     combinedStatus = JSON.parse(q_Service.getCombinedStatus())
    //     console.log(JSON.stringify(combinedStatus.hostapd_status, null, 2));
    //     console.log(combinedStatus.hostapd_conf);
    //     console.log(JSON.stringify(combinedStatus.dnsmasq_status, null, 2));
    //     console.log(combinedStatus.dnsmasq_conf);
    //     console.log(JSON.stringify(combinedStatus.iptables_status, null, 2));
    // }
    // Rectangle {
    //     width: parent.width
    //     height: 30
    //     Text {
    //         text: combinedStatus.hostapd_status.Active
    //         anchors.centerIn: parent
    //     }
    // }

    Row {
        SideMenu {
            id: sideMenu
            current: "q_service"
        }
        Item {
            visible: true
            width: window.width - sideMenu.width ; height: window.height

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
