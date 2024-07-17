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

    // ListModel {
    //     id: networkInfoModel
    // }

    // Component.onCompleted: {
    //     var json = q_Network.getNetworkInfoAsJson()
    //     console.log(json)
    //     // networkInfoModel = JSON.parse(json).network_interfaces

    //     networkInfoModel.append(JSON.parse(json).network_interfaces[0])
    // }

    Row {
        
        SideMenu {
            current: "q_network"
        }

        Item {
            visible: true
            width: 700; height: 500

            ColumnLayout {
                anchors.fill: parent
                spacing: 1
                property var currentItem: null

                Repeater {
                    model: JSON.parse(q_Network.getNetworkInfoAsJson()).network_interfaces
                    delegate: AccordionSection {
                        required property var modelData
                        title: "[" + modelData.nmcli_info["GENERAL.TYPE"]+"] " + modelData.udevadm_info["ID_MODEL_FROM_DATABASE"]
                        contentItem: Rectangle {

                            Component.onCompleted: {
                                function udevadm_info_append(name){
                                    networkInfoModel.append({name: name, value: modelData.udevadm_info[name]})
                                }
                                udevadm_info_append("ID_BUS")
                                udevadm_info_append("ID_MODEL_FROM_DATABASE")
                                udevadm_info_append("ID_VENDOR_FROM_DATABASE")
                                udevadm_info_append("ID_OUI_FROM_DATABASE")
                                console.log(JSON.stringify(modelData, null, 2));
                            }
                            anchors.fill: parent

                            ScrollView {
                                width: parent.width
                                height: parent.height
                                ListView {
                                    model: ListModel {
                                        id: networkInfoModel
                                    }
                                    delegate: RowLayout {
                                        Rectangle {
                                            width: 260
                                            height: 30
                                            Text {
                                                text: name
                                                anchors.centerIn: parent
                                            }
                                        }
                                        Text {
                                            text: value
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
