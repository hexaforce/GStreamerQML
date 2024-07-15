TEMPLATE = app

QT += qml quick widgets

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig debug
PKGCONFIG += \
    gstreamer-1.0 \
    gstreamer-video-1.0 \
    protobuf

DEFINES += GST_USE_UNSTABLE_API

# INCLUDEPATH += ../lib
# INCLUDEPATH += message
INCLUDEPATH += src

HEADERS += src/pipeline_thread.h \
           src/process_runner.h \
           src/UdpReceiver.h

SOURCES += main.cpp \
           src/pipeline_thread.cpp  \
           src/process_runner.cpp \
           src/UdpReceiver.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Include the directory where protoc generated the headers and sources
INCLUDEPATH += .
