TEMPLATE = app
QT -= gui
CONFIG += console
CONFIG -= app_bundle

SUBDIRS += common
SUBDIRS += transmitter

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-1.0

SOURCES += main.cpp
SOURCES += transmit_pipelines.cpp

HEADERS += transmit_pipelines.h

INCLUDEPATH += .

# Include the common directory
INCLUDEPATH += ../common

# Link the common library
LIBS += -L$$OUT_PWD/../common -lcommon
