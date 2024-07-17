import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import jp.fpv.Q_Network 1.0 

Item {
    focus: true

    Q_Network {
        id: q_Network
    }

    Component.onCompleted: {
        console.log(q_Network.getNetworkInfoAsJson())
    }

    Row {
        
        SideMenu {
            current: "q_network"
        }

        Item {
            visible: true
            width: 600; height: 500

            ColumnLayout {
                anchors.fill: parent
                spacing: 1
                property var currentItem: null
                Repeater {
                    model: JSON.parse(q_Network.getNetworkInfoAsJson()).network_interfaces
                    delegate: AccordionSection {
                        required property var modelData
                        title: "[" +modelData.nmcli_info["GENERAL.TYPE"]+"] " +modelData.udevadm_info["ID_MODEL_FROM_DATABASE"]
                        contentItem: Rectangle {
                            anchors.fill: parent
                            ListView {
                                width: 180; height: 200

                                model: ListModel {
                                    ListElement {
                                        key: "key1"
                                        value: "value1"
                                    }
                                    ListElement {
                                        key: "key22222222222"
                                        value: "value2"
                                    }
                                    ListElement {
                                        key: "key3"
                                        value: "value3"
                                    }
                                }

                                delegate: Item {
                                    width: 180
                                    height: 40
                                    RowLayout {
                                        anchors.fill: parent
                                        Text {
                                            text: key
                                            Layout.alignment: Qt.AlignLeft
                                            Layout.fillWidth: true
                                        }
                                        Text {
                                            text: value
                                            Layout.alignment: Qt.AlignRight
                                        }
                                    }
                                }
                            }
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
