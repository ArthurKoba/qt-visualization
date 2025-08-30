QT += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = COM_Serial_Viewer
TEMPLATE = app

SOURCES += \
    main.cpp \
    serial_viewer.cpp \
    serial_worker.cpp

HEADERS += \
    serial_viewer.h \
    serial_worker.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Windows specific settings
win32 {
    RC_ICONS = app_icon.ico
    VERSION = 1.0.0.0
    QMAKE_TARGET_COMPANY = "Serial Tools"
    QMAKE_TARGET_PRODUCT = "COM Serial Viewer"
    QMAKE_TARGET_DESCRIPTION = "COM Port Serial Data Viewer with Minimal Latency"
    QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2024"
}

# Optimization flags for minimal latency
QMAKE_CXXFLAGS_RELEASE += -O2
QMAKE_CXXFLAGS_DEBUG += -O0 -g