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

    property var combinedStatus: null
    property var forwardHead: null

    ListModel {
        id: forwardRule
    }


    Component.onCompleted: {
        combinedStatus = JSON.parse(q_Network.getCombinedStatus())
        // console.log(JSON.stringify(combinedStatus.hostapd_status, null, 2));
        // console.log(combinedStatus.hostapd_conf);
        // console.log(JSON.stringify(combinedStatus.dnsmasq_status, null, 2));
        // console.log(combinedStatus.dnsmasq_conf);
        // console.log(JSON.stringify(combinedStatus.ufw_status, null, 2));

        var FORWARD = combinedStatus.iptables_status.FORWARD.filter(function(rule) { return rule.length !== 0; }).map(function(rule) { return rule[0]; })
        forwardHead = Object.keys(FORWARD[0])
        forwardRule.clear();
        for (var i = 0; i < FORWARD.length; i++) {
            forwardRule.append(FORWARD[i])
        }
        // console.log(forwardHead)

        // var INPUT = combinedStatus.iptables_status.INPUT
        // .filter(function(rule) { return rule.length !== 0; })
        // .map(function(rule) { return rule[0]; })
        // // console.log(JSON.stringify(INPUT, null, 2))

        // var OUTPUT = combinedStatus.iptables_status.OUTPUT
        // .filter(function(rule) { return rule.length !== 0; })
        // .map(function(rule) { return rule[0]; })
        // // console.log(JSON.stringify(OUTPUT, null, 2))

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
            current: "q_service"
        }
        Item {
            visible: true
            width: window.width - sideMenu.width ; height: window.height


            ColumnLayout {
                anchors.fill: parent
                spacing: 1
                property var currentItem: null

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
                                Button {
                                    text: "start"
                                    width: 100
                                    height: 30
                                    onClicked: {
                                        // ボタン1のクリック処理
                                    }
                                }
                                Button {
                                    text: "stop"
                                    width: 100
                                    height: 30
                                    onClicked: {
                                        // ボタン2のクリック処理
                                    }
                                }
                                Button {
                                    text: "restart"
                                    width: 100
                                    height: 30
                                    onClicked: {
                                        // ボタン3のクリック処理
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
                                Button {
                                    text: "start"
                                    width: 100
                                    height: 30
                                    onClicked: {
                                        // ボタン1のクリック処理
                                    }
                                }
                                Button {
                                    text: "stop"
                                    width: 100
                                    height: 30
                                    onClicked: {
                                        // ボタン2のクリック処理
                                    }
                                }
                                Button {
                                    text: "restart"
                                    width: 100
                                    height: 30
                                    onClicked: {
                                        // ボタン3のクリック処理
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
                            Row {
                                spacing: 5
                                ComboBox {
                                    id: networkComboBox
                                    currentIndex: 0
                                    model: forwardHead
                                    height: 30
                                }
                                Button {
                                    text: "add"
                                    width: 100
                                    height: 30
                                    onClicked: {
                                        // ボタン1のクリック処理
                                        console.log("Current text:", networkComboBox.model[networkComboBox.currentIndex])
                                    }
                                }
                            }
                            Item{
                                width: firewall.width ; height: firewall.height
                                Column {
                                    Row {
                                        Repeater {
                                            model: forwardHead
                                            delegate: Rectangle {
                                                width: modelData === "target" ? 150 : 75
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
                                                    width: modelData === "target" ? 150 : 75
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
