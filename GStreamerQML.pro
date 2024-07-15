TEMPLATE = app

QT += qml quick widgets

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig debug
PKGCONFIG += \
    gstreamer-1.0 \
    gstreamer-video-1.0 \
    protobuf

DEFINES += GST_USE_UNSTABLE_API

INCLUDEPATH += ../lib
INCLUDEPATH += message

HEADERS += set_playing.h \
           process_runner.h 

SOURCES += main.cpp \
           process_runner.cpp 

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Include the directory where protoc generated the headers and sources
INCLUDEPATH += .
