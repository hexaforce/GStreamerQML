QT += qml quick multimedia

CONFIG += c++11

SOURCES += main.cpp

RESOURCES += qml.qrc

unix:!macx {
    LIBS += -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
    LIBS += -lQt5MultimediaQuick -lQt5Quick -lQt5Multimedia -lQt5Gui -lQt5QmlModels -lQt5Qml -lQt5Network -lQt5Core -lGL -lpthread
}

# Additional build configuration for other platforms
