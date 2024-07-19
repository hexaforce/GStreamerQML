import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

// import jp.fpv.NetworkSetting 1.0
// import jp.fpv.CommonPipelineManager 1.0

Item {
    focus: true


    property var combinedStatus: null
    property var forwardHead: ["num","pkts","bytes","target","prot","opt","in","out","source","destination"]
    property var networkInterfaces: null


    ListModel {
        id: inputRule
    }

    ListModel {
        id: forwardRule
    }

    ListModel {
        id: outputRule
    }

    // CommonPipelineManager {
    //     id: commonPipelineManager
    //     objectName: "commonPipelineManager"
    // }

    function appendListModel(json, listModel){
        var temp = json.filter(function(rule) { return rule.length !== 0; }).map(function(rule) { return rule[0]; })
        listModel.clear()
        for (var i = 0; i < temp.length; i++) {
            listModel.append(temp[i])
        }
    }

    Component.onCompleted: {
        combinedStatus = JSON.parse(commonNetworkService.getCombinedStatus())
        appendListModel(combinedStatus.iptables_status.OUTPUT, inputRule)
        appendListModel(combinedStatus.iptables_status.FORWARD, forwardRule)
        appendListModel(combinedStatus.iptables_status.OUTPUT, outputRule)
        networkInterfaces = JSON.parse(commonNetworkService.getNetworkInfoAsJson()).network_interfaces
        // console.log(JSON.stringify(networkInterfaces, null, 2))
    }

    function statusColor(statusText){
        if (statusText.includes("active (running)") || statusText.includes("active (exited)")) {
            return "green";
        } else if (statusText.includes("failed") || statusText.includes("activating (auto-restart)")) {
            return "red";
        } else {
            return "gray";
        }
    }

    Row {
        SideMenu {
            id: sideMenu
            current: "servicesetting"
        }
        Item {
            visible: true
            width: window.width - sideMenu.width ; height: window.height


            ColumnLayout {
                anchors.fill: parent
                spacing: 1
                property var currentItem: null

                Text {
                    text: qsTr("Reception service setting (Current machine")
                }

                AccordionSection {
                    title: "Access point and authentication server for Wi-Fi and Ethernet"
                    Rectangle {
                        Column {
                            Text {
                                text: "hostapd.service"
                                height: 30
                            }
                            Text {
                                text: combinedStatus.hostapd_status.Active
                                color: statusColor(combinedStatus.hostapd_status.Active)
                                height: 30
                            }
                            Row {
                                spacing:5
                                Repeater {
                                    model: ["start","stop","restart","unmask","status","enable","disable"]
                                    delegate: Button {
                                        text: modelData
                                        width: 100
                                        height: 30
                                        onClicked: {
                                            processRunner.runCommand("sudo", ["systemctl", modelData, "hostapd"])
                                        }
                                    }
                                }
                            }

                            Text {
                                height: 30
                                text: "/etc/hostapd/hostapd.conf"
                                verticalAlignment: Text.AlignBottom
                            }
                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "lightgray"
                            }
                            TextArea {
                                id: hostapd_conf
                                wrapMode: TextArea.Wrap
                                text: combinedStatus.hostapd_conf
                            }
                            Button {
                                text: "update"
                                width: 100
                                height: 30
                                onClicked: {
                                    // ボタン1のクリック処理
                                    console.log("Current text:", hostapd_conf.text)
                                }
                            }
                        }
                    }
                }
                AccordionSection {
                    title: "A lightweight DHCP and caching DNS server"
                    Rectangle {
                        Column {
                            Text {
                                text: "dnsmasq.service"
                                height: 30
                            }
                            Text {
                                text: combinedStatus.dnsmasq_status.Active
                                color: statusColor(combinedStatus.dnsmasq_status.Active)
                                height: 30
                            }
                            Row {
                                spacing: 5
                                Repeater {
                                    model: ["start","stop","restart","unmask","status","enable","disable"]
                                    delegate: Button {
                                        text: modelData
                                        width: 100
                                        height: 30
                                        onClicked: {
                                            processRunner.runCommand("sudo", ["systemctl", modelData, "dnsmasq"])
                                        }
                                    }
                                }
                            }
                            Row {
                                spacing: 5

                                Label {
                                    height: 30
                                    text: "interface:"
                                }
                                ComboBox {
                                    id: interfaceComboBox
                                    currentIndex: 0
                                    model: networkInterfaces.filter(interfaces => interfaces.nmcli_info["GENERAL.TYPE"] === "wifi").map(interfaces => interfaces.nmcli_info["GENERAL.DEVICE"])
                                    width: 180
                                    height: 30
                                }
                                Label {
                                    height: 30
                                    text: "address:"
                                }
                                TextField {
                                    id: address
                                     width: 130
                                    height: 30
                                    placeholderText: qsTr("192.168.4.10")
                                }

                                Label {
                                    height: 30
                                    text: "netmask:"
                                }
                                TextField {
                                    id: netmask
                                    width: 130
                                    height: 30
                                    placeholderText: qsTr("255.255.255.0")
                                }
                                Button {
                                    text: "attch ip adress"
                                    width: 160
                                    height: 30
                                    onClicked: {
                                        let interfaces = interfaceComboBox.model[interfaceComboBox.currentIndex]
                                        processRunner.runCommand("sudo", ["ifconfig", interfaces, address.text, "netmask", netmask.text, "up" ])
                                    }
                                }
                            }

                            Text {
                                height: 30
                                text: "/etc/dnsmasq.conf"
                                verticalAlignment: Text.AlignBottom
                            }
                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "lightgray"
                            }
                            TextArea {
                                id: dnsmasq_conf
                                wrapMode: TextArea.Wrap
                                text: combinedStatus.dnsmasq_conf
                            }
                            Button {
                                text: "update"
                                width: 100
                                height: 30
                                onClicked: {
                                    // ボタン1のクリック処理
                                    console.log("Current text:", dnsmasq_conf.text)
                                }
                            }
                        }
                    }
                }
                AccordionSection {
                    id: firewall
                    title: "Uncomplicated firewall"
                    Rectangle {
                        Column {
                            Text {
                                text: "ufw.service"
                                height: 30
                            }
                            Text {
                                text: combinedStatus.ufw_status.Active
                                color: statusColor(combinedStatus.ufw_status.Active)
                                height: 30
                            }
                            Text {
                                text: "Access point traffic"
                                height: 30
                                verticalAlignment: Text.AlignBottom
                            }
                            Row {
                                spacing: 5
                                Label {
                                    height: 30
                                    text: "AP:"
                                }
                                ComboBox {
                                    id: accessPointComboBox
                                    currentIndex: 0
                                    model: networkInterfaces.filter(interfaces => interfaces.nmcli_info["GENERAL.TYPE"] === "wifi").map(interfaces => interfaces.nmcli_info["GENERAL.DEVICE"])
                                    width: 180
                                    height: 30
                                }
                                Button {
                                    text: "allow inbound/outbound"
                                    width: 200
                                    height: 30
                                    onClicked: {
                                        let interfaces = accessPointComboBox.model[accessPointComboBox.currentIndex]
                                        processRunner.runCommand("sudo", ["iptables", "-A", "FORWARD", "-i", interfaces, "-j", "ACCEPT"])
                                        processRunner.runCommand("sudo", ["iptables", "-A", "FORWARD", "-o", interfaces, "-j", "ACCEPT"])
                                    }
                                }
                            }
                            Text {
                                text: "Internet access via access point"
                                height: 30
                                verticalAlignment: Text.AlignBottom
                            }
                            Row {
                                spacing: 5
                                Label {
                                    height: 30
                                    text: "AP:"
                                    // Layout.alignment: Qt.AlignRight
                                    // horizontalAlignment: Text.AlignHCenter
                                }
                                ComboBox {
                                    id: apComboBox
                                    currentIndex: 0
                                    model: networkInterfaces.filter(interfaces => interfaces.nmcli_info["GENERAL.TYPE"] === "wifi").map(interfaces => interfaces.nmcli_info["GENERAL.DEVICE"])
                                    width: 180
                                    height: 30
                                }
                                Label {
                                    height: 30
                                    text: "WAN:"
                                }
                                ComboBox {
                                    id: wanComboBox
                                    currentIndex: 0
                                    model: networkInterfaces.map(interfaces => interfaces.nmcli_info["GENERAL.DEVICE"])
                                    width: 180
                                    height: 30
                                }
                                Button {
                                    text: "allow internet access"
                                    width: 200
                                    height: 30
                                    onClicked: {
                                        let apInterfaces = apComboBox.model[apComboBox.currentIndex]
                                        let wanInterfaces = wanComboBox.model[wanComboBox.currentIndex]
                                        processRunner.runCommand("sudo", ["iptables", "-t", "nat", "-A", "POSTROUTING", "-o", wanInterfaces, "-j", "MASQUERADE"])
                                        processRunner.runCommand("sudo", ["iptables", "-A", "FORWARD", "-i", apInterfaces, "-o", wanInterfaces, "-j", "ACCEPT"])
                                        processRunner.runCommand("sudo", ["iptables", "-A", "FORWARD", "-i", wanInterfaces, "-o", apInterfaces, "-m", "state", "--state", "RELATED,ESTABLISHED", "-j", "ACCEPT"])
                                    }
                                }
                            }

                            // Text {
                            //     text: "INPUT"
                            //     height: 30
                            //     verticalAlignment: Text.AlignBottom
                            // }
                            // Item{
                            //     width: firewall.width ; height: firewall.height
                            //     Column {
                            //         Row {
                            //             Repeater {
                            //                 model: forwardHead
                            //                 delegate: Rectangle {
                            //                     // width: modelData === "target" ? 150 : 75
                            //                     width: 85
                            //                     height: 30
                            //                     color: "lightgray"
                            //                     border.color: "black"
                            //                     border.width: .5
                            //                     Text {
                            //                         anchors.centerIn: parent
                            //                         text: modelData;
                            //                         font.bold: true
                            //                     }
                            //                 }
                            //             }
                            //         }
                            //         ListView {
                            //             width: firewall.width ; height: firewall.height
                            //             model: inputRule
                            //             delegate: Row {
                            //                 property var rule: model
                            //                 Repeater {
                            //                     model: forwardHead
                            //                     delegate: Rectangle {
                            //                         // width: modelData === "target" ? 150 : 75
                            //                         width: 85
                            //                         height: 30
                            //                         border.color: "black"
                            //                         border.width: .5
                            //                         Text {
                            //                             anchors.centerIn: parent
                            //                             text: rule[modelData]
                            //                         }
                            //                     }
                            //                 }
                            //             }
                            //         }
                            //     }
                            // }

                            Text {
                                text: "FORWARD"
                                height: 30
                                verticalAlignment: Text.AlignBottom
                            }
                            Item{
                                width: firewall.width ; height: firewall.height
                                Column {
                                    Row {
                                        Repeater {
                                            model: forwardHead
                                            delegate: Rectangle {
                                                // width: modelData === "target" ? 150 : 75
                                                width: 85
                                                height: 30
                                                color: "lightgray"
                                                border.color: "black"
                                                border.width: .5
                                                Text {
                                                    anchors.centerIn: parent
                                                    text: modelData;
                                                    font.bold: true
                                                }
                                            }
                                        }
                                    }
                                    ListView {
                                        width: firewall.width ; height: firewall.height
                                        model: forwardRule
                                        delegate: Row {
                                            property var rule: model
                                            Repeater {
                                                model: forwardHead
                                                delegate: Rectangle {
                                                    // width: modelData === "target" ? 150 : 75
                                                    width: 85
                                                    height: 30
                                                    border.color: "black"
                                                    border.width: .5
                                                    Text {
                                                        anchors.centerIn: parent
                                                        text: rule[modelData]
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            // Text {
                            //     text: "OUTPUT"
                            //     height: 30
                            //     verticalAlignment: Text.AlignBottom
                            // }
                            // Item{
                            //     width: firewall.width ; height: firewall.height
                            //     Column {
                            //         Row {
                            //             Repeater {
                            //                 model: forwardHead
                            //                 delegate: Rectangle {
                            //                     // width: modelData === "target" ? 150 : 75
                            //                     width: 85
                            //                     height: 30
                            //                     color: "lightgray"
                            //                     border.color: "black"
                            //                     border.width: .5
                            //                     Text {
                            //                         anchors.centerIn: parent
                            //                         text: modelData;
                            //                         font.bold: true
                            //                     }
                            //                 }
                            //             }
                            //         }
                            //         ListView {
                            //             width: firewall.width ; height: firewall.height
                            //             model: outputRule
                            //             delegate: Row {
                            //                 property var rule: model
                            //                 Repeater {
                            //                     model: forwardHead
                            //                     delegate: Rectangle {
                            //                         // width: modelData === "target" ? 150 : 75
                            //                         width: 85
                            //                         height: 30
                            //                         border.color: "black"
                            //                         border.width: .5
                            //                         Text {
                            //                             anchors.centerIn: parent
                            //                             text: rule[modelData]
                            //                         }
                            //                     }
                            //                 }
                            //             }
                            //         }
                            //     }
                            // }

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
