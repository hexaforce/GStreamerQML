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

HEADERS += src/pipeline_manager.h \
           src/process_runner.h \
           src/udp_receiver.h \
           src/device_monitor.h \
           src/receive_pipelines.h \
           src/pipeline_controller.h \
           src/q_network.h \
           src/q_video.h \
           src/q_audio.h \
           src/q_telemetry.h

SOURCES += main.cpp \
           src/pipeline_manager.cpp  \
           src/process_runner.cpp \
           src/udp_receiver.cpp \
           src/device_monitor.cpp \
           src/receive_pipelines.cpp \
           src/pipeline_controller.cpp \
           src/q_network.cpp \
           src/q_video.cpp \
           src/q_audio.cpp \
           src/q_telemetry.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Include the directory where protoc generated the headers and sources
INCLUDEPATH += .
