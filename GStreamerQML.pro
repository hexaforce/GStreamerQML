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

HEADERS += src/common_pipeline_manager.h \
           src/receive_pipelines.h \
           src/common_process_runner.h \
           src/common_telemetry_socket.h \
           src/common_device_monitor.h \
           src/common_network_service.h 

SOURCES += main.cpp \
           src/common_pipeline_manager.cpp \
           src/receive_pipelines.cpp \
           src/common_process_runner.cpp \
           src/common_telemetry_socket.cpp \
           src/common_device_monitor.cpp \
           src/common_network_service.cpp 

RESOURCES += main.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Include the directory where protoc generated the headers and sources
INCLUDEPATH += .
