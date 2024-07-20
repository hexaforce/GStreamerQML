TEMPLATE = subdirs

SUBDIRS += common
#SUBDIRS += transmitter
SUBDIRS += receiver

# Define common include paths, library paths, and other settings here if needed
# For example, if both subprojects share the same include path:
# INCLUDEPATH += /path/to/shared/include

# Common configuration settings (optional)
# CONFIG += c++11
# QMAKE_CXXFLAGS += -Wall -Wextra

# Define additional build steps or configurations if needed
# For example, custom commands for pre- or post-build steps
# QMAKE_POST_LINK += custom_command
