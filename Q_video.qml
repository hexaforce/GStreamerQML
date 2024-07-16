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
            current: "q_video"
        }

        Column {
            // ComboBox {
            //     id: cameraComboBox
            //     width: 240
            //     // model: q_Video.getVideoDevices()
            //     onCurrentIndexChanged: {

            //     }
            // }
            Rectangle {
                width: 400
                height: 300
                color: "blue"
            }
        }
    }
}
