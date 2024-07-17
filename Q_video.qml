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
            id: sideMenu
            current: "q_video"
        }
        Item {
            visible: true
            width: window.width - sideMenu.width ; height: window.height

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
                            anchors.fill: parent
                            Column{

                                ComboBox {
                                    id: mediaComboBox
                                    currentIndex: 0
                                    model: Array.from(new Set(modelData.caps.map(cap => cap.media)))
                                    onCurrentIndexChanged: {
                                        let media = mediaComboBox.model[currentIndex]
                                        resolutionComboBox.model = modelData.caps.filter(cap => cap.media === media).map(cap => cap.width+"x"+cap.height).reverse()
                                    }
                                }

                                ComboBox {
                                    id: resolutionComboBox
                                    currentIndex: 0
                                    model:[]
                                    onCurrentIndexChanged: {
                                        let media = mediaComboBox.currentText
                                        let resolution = resolutionComboBox.model[currentIndex].split("x")
                                        let width = Number(resolution[0])
                                        let height = Number(resolution[1])
                                        framerateComboBox.model = modelData.caps.filter(cap => cap.media === media && cap.width === width)[0].framerate
                                    }
                                }

                                ComboBox {
                                    id: framerateComboBox
                                    currentIndex: 0
                                    model:[]
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
