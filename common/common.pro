TEMPLATE = lib
CONFIG += shared
QT += core

CONFIG += c++11
CONFIG += link_pkgconfig

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig debug
PKGCONFIG += \
    gstreamer-1.0 \
    gstreamer-video-1.0

HEADERS += common_device_monitor.h
HEADERS += common_network_service.h
HEADERS += common_pipeline_manager.h
HEADERS += common_process_runner.h
HEADERS += common_telemetry_socket.h

SOURCES += common_device_monitor.cpp
SOURCES += common_network_service.cpp
SOURCES += common_pipeline_manager.cpp
SOURCES += common_process_runner.cpp
SOURCES += common_telemetry_socket.cpp

# LIBS += ...
# INCLUDEPATH += ...
