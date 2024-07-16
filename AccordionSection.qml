import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    width: parent.width

    property alias title: sectionTitle.text
    default property alias contentData: sectionContent.data

    signal toggled(bool expanded)

    Rectangle {
        id: header
        width: parent.width
        height: 40
        color: "lightgray"
        border.color: "gray"
        MouseArea {
            id: headerMouseArea
            anchors.fill: parent
            onClicked: sectionContent.visible = !sectionContent.visible
        }

        Text {
            id: sectionTitle
            anchors.centerIn: parent
        }
    }

    Column {
        id: sectionContent
        width: parent.width
        visible: false

        anchors.top: header.bottom
    }

    onToggled: {
        sectionContent.visible = expanded
    }
    
}
