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
                            Text {
                                text: "/etc/hostapd/hostapd.conf"
                            }
                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "lightgray"
                            }
                            Text {
                                text: combinedStatus.hostapd_conf
                                wrapMode: Text.Wrap
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
                            Text {
                                text: "/etc/dnsmasq.conf"
                            }
                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "lightgray"
                            }
                            Text {
                                text: combinedStatus.dnsmasq_conf
                                wrapMode: Text.Wrap
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
                            Item{
                                width: firewall.width ; height: firewall.height
                                Column {
                                    Row {
                                        Repeater {
                                            model: forwardHead
                                            delegate: Text { text: modelData; font.bold: true; width: 100 }
                                        }
                                    }
                                    ListView {
                                        width: firewall.width ; height: firewall.height
                                        model: forwardRule
                                        delegate: Row {
                                            property var rule: model
                                            Repeater {
                                                model: forwardHead
                                                delegate: Text { text: rule[modelData];  width: 100 }
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
