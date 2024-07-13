import QtQuick 2.15
import QtQuick.Window 2.15
import QtMultimedia 5.15

Window {
    visible: true
    width: 640
    height: 480
    title: "GStreamer in QML"

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
        source: mediaPlayer
    }

    MediaPlayer {
        id: mediaPlayer
        autoPlay: true
        source: "videotestsrc ! videoconvert ! qmlglsink"
    }
}
