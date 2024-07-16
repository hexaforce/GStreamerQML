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

    Row {
        
        SideMenu {
            current: "q_network"
        }

        Column {
            ComboBox {
                id: deviceComboBox
                width: 240
                model: q_Network.getWifiDevices()
                onCurrentIndexChanged: {

                }
            }
            Rectangle {
                width: 400
                height: 300
                color: "red"
            }
        }
    }
}
