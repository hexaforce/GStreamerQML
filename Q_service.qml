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
    Component.onCompleted: {
        combinedStatus = JSON.parse(q_Network.getCombinedStatus())
        console.log(JSON.stringify(combinedStatus.hostapd_status, null, 2));
        console.log(combinedStatus.hostapd_conf);
        console.log(JSON.stringify(combinedStatus.dnsmasq_status, null, 2));
        console.log(combinedStatus.dnsmasq_conf);
        console.log(JSON.stringify(combinedStatus.ufw_status, null, 2));
        console.log(JSON.stringify(combinedStatus.iptables_status, null, 2));
    }
    // Rectangle {
    //     width: parent.width
    //     height: 30
    //     Text {
    //         text: combinedStatus.hostapd_status.Active
    //         anchors.centerIn: parent
    //     }
    // }
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
                    title: "hostapd.service"
                    Rectangle {
                        Column {
                            Text {
                                text: "Access point and authentication server for Wi-Fi and Ethernet"
                                height: 30
                            }
                            Text {
                                text: combinedStatus.hostapd_status.Active
                                color: statusColor(combinedStatus.hostapd_status.Active)
                                height: 30
                            }
                        }
                    }
                }
                AccordionSection {
                    title: "dnsmasq.service"
                    Rectangle {
                        Column {
                            Text {
                                text: "A lightweight DHCP and caching DNS server"
                                height: 30
                            }
                            Text {
                                text: combinedStatus.dnsmasq_status.Active
                                color: statusColor(combinedStatus.dnsmasq_status.Active)
                                height: 30
                            }
                        }
                    }
                }
                AccordionSection {
                    title: "ufw.service"
                    Rectangle {
                        Column {
                            Text {
                                text: "Uncomplicated firewall"
                                height: 30
                            }
                            Text {
                                text: combinedStatus.ufw_status.Active
                                color: statusColor(combinedStatus.ufw_status.Active)
                                height: 30
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
