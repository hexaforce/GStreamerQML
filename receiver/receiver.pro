TEMPLATE = app
QT += qml quick widgets

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig debug
PKGCONFIG += \
    gstreamer-1.0 \
    gstreamer-video-1.0

DEFINES += GST_USE_UNSTABLE_API

# INCLUDEPATH += ../lib
# INCLUDEPATH += message
INCLUDEPATH += src

HEADERS += src/receive_pipelines.h

SOURCES += main.cpp \
           src/receive_pipelines.cpp

RESOURCES += main.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Include the directory where protoc generated the headers and sources
INCLUDEPATH += .

# Include the common directory
INCLUDEPATH += ../common

# Link the common library
LIBS += -L$$OUT_PWD/../common -lcommon
