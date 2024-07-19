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

    property var pipelineElements: null

    Component.onCompleted: {
        pipelineElements = JSON.parse(pipelineManager.getPipelineInfo()).elements.reverse()
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

                // var modelData =  JSON.parse(pipelineManager.getPipelineInfo()).elements
                // console.log(JSON.stringify(modelData, null, 2))


                Text {
                    text: qsTr("Reception pipline setting (Current machine")
                }

                Repeater {
                    model: pipelineElements
                    delegate: AccordionSection {
                        required property var modelData
                        property int index: pipelineElements.findIndex(element => element.name === modelData.name)
                        title: "    ".repeat(index) + " > " + modelData.type + " (" + modelData.name + ")"
                        contentItem: Rectangle {
                            anchors.fill: parent
                            Row{
                                // Component.onCompleted: selectManager()
                                spacing: 5
                                Text {
                                    text: qsTr(JSON.stringify(modelData.properties, null, 2))

                                }
                            }
                        }
                    }
                }

                Text {
                    text: qsTr("Reception pipline setting (Current machine")
                }

                Repeater {
                    model: JSON.parse(deviceMonitor.listDevices("Video"))
                    delegate: AccordionSection {
                        required property var modelData
                        title: modelData.Device

                        function selectManager(){
                            mediaComboBox.model = Array.from(new Set(modelData.caps.map(cap => cap.media)))

                            let media = mediaComboBox.model[mediaComboBox.currentIndex]
                            resolutionComboBox.model = modelData.caps.filter(cap => cap.media === media).map(cap => cap.width+"x"+cap.height)

                            let resolution = resolutionComboBox.model[resolutionComboBox.currentIndex].split("x")
                            let width = Number(resolution[0])
                            let height = Number(resolution[1])
                            framerateComboBox.model = modelData.caps.filter(cap => cap.media === media && cap.width === width)[0].framerate
                        }

                        contentItem: Rectangle {
                            anchors.fill: parent
                            Row{
                                Component.onCompleted: selectManager()
                                spacing: 5
                                ComboBox {
                                    id: mediaComboBox
                                    height: 30
                                    currentIndex: 0
                                    model:[]
                                    onCurrentIndexChanged: {selectManager()}
                                }
                                ComboBox {
                                    id: resolutionComboBox
                                    height: 30
                                    currentIndex: 0
                                    model:[]
                                    onCurrentIndexChanged: selectManager()
                                }
                                ComboBox {
                                    id: framerateComboBox
                                    height: 30
                                    currentIndex: 0
                                    model:[]
                                    onCurrentIndexChanged: selectManager()
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
