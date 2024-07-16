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
            width: 600
            height: 500

            ColumnLayout {
                anchors.fill: parent
                spacing: 1
                property var currentItem: null
                Repeater {
                    model: JSON.parse(q_Network.getNetworkInfoAsJson()).network_interfaces
                    delegate: AccordionSection {
                        required property var modelData
                        title: modelData.nmcli_info["GENERAL.DEVICE"]
                        contentItem: Rectangle {
                            anchors.fill: parent
                            Column{

                                // ComboBox {
                                //     id: mediaComboBox
                                //     currentIndex: 0
                                //     model: Array.from(new Set(modelData.caps.map(cap => cap.media)))
                                //     onCurrentIndexChanged: {
                                //         let media = mediaComboBox.model[currentIndex]
                                //         resolutionComboBox.model = modelData.caps.filter(cap => cap.media === media).map(cap => cap.width+"x"+cap.height).reverse()
                                //     }
                                // }

                                // ComboBox {
                                //     id: resolutionComboBox
                                //     currentIndex: 0
                                //     model:[]
                                //     onCurrentIndexChanged: {
                                //         let media = mediaComboBox.currentText
                                //         let resolution = resolutionComboBox.model[currentIndex].split("x")
                                //         let width = Number(resolution[0])
                                //         let height = Number(resolution[1])
                                //         framerateComboBox.model = modelData.caps.filter(cap => cap.media === media && cap.width === width)[0].framerate
                                //     }
                                // }

                                // ComboBox {
                                //     id: framerateComboBox
                                //     currentIndex: 0
                                //     model:[]
                                // }

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



        // Column {
        //     ComboBox {
        //         id: deviceComboBox
        //         width: 240
        //         model: q_Network.getNetworkInfoAsJson()
        //         onCurrentIndexChanged: {

        //         }
        //     }
        //     Rectangle {
        //         width: 400
        //         height: 300
        //         color: "red"
        //     }
        // }
    }
}
