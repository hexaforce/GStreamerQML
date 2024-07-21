import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

Item {
    focus: true

    Row {
        SideMenu {
            id: sideMenu
            current: "setting_audio"
        }
        Item {
            visible: true
            width: window.width - sideMenu.width ; height: window.height

            ColumnLayout {
                anchors.fill: parent
                spacing: 1
                property var currentItem: null
                Repeater {
                    model: JSON.parse(deviceMonitor.listDevices("Audio"))
                    delegate: AccordionSection {
                        required property var modelData
                        title: modelData.Device
                        contentItem: Rectangle {
                            anchors.fill: parent
                            Row{
                                spacing: 5
                                ComboBox {
                                    id: mediaComboBox
                                    height: 30
                                    currentIndex: 0
                                    model: Array.from(new Set(modelData.caps.map(cap => cap.media)))
                                    onCurrentIndexChanged: {
                                        let media = mediaComboBox.model[currentIndex]
                                        let cap = modelData.caps.filter(cap => cap.media === media)[0]
                                        if ("format" in cap)
                                            formatComboBox.model = cap.format
                                    }
                                }
                                ComboBox {
                                    id: formatComboBox
                                    height: 30
                                    currentIndex: 0
                                    model:[]
                                    onCurrentIndexChanged: {
                                        let media = mediaComboBox.currentText

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
