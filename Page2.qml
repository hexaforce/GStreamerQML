import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

Item {
    focus: true
    width: 960
    height: 540
    Row {
        Column {
            Text {
                text: "This is page2"
            }
            Button {
                text: "Go Back"
                Material.foreground: Material.Primary
                onClicked: stack.pop(stack.find(function(item) {
                    return item.name === "page2"
                }))
            }
            Button {
                text: "Go page1"
                Material.foreground: Material.Primary
                onClicked: stack.push(page1)
            }
            Button {
                text: "Go page2"
                Material.foreground: Material.Primary
                onClicked: stack.push(page2)
                enabled: false
                opacity: enabled ? 1.0 : 0.75
            }
        }
        Column {
            Rectangle {
                width: 400
                height: 300
                color: "blue"
            }
        }
    }
}
