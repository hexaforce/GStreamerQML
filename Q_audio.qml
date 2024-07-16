import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import jp.fpv.Q_Audio 1.0

Item {
    focus: true

    Q_Audio {
        id: q_Audio
    }

    Row {

        SideMenu {
            current: "q_audio"
        }

        Column {
            Button {
                text: "List Audio Devices"
                onClicked: {
                    console.log(deviceMonitor.listDevices("Audio"))
                }
            }
            Button {
                text: "List Video Devices"
                onClicked: {
                    console.log(deviceMonitor.listDevices("Video"))
                }
            }
            Rectangle {
                width: 400
                height: 300
                color: "green"
            }
        }
    }
}
