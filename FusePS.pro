QT += core multimedia
QT -= gui

CONFIG += c++11

QMAKE_CXXFLAGS += -D_FILE_OFFSET_BITS=64 -D_DARWIN_USE_64_BIT_INODE
LIBS += -L/usr/local/lib -losxfuse
INCLUDEPATH += /usr/local/include/osxfuse/fuse

TARGET = FusePS
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    GlobalInfo.cpp \
    PSStatus.cpp \
    tool.cpp \
    BasePSConfig.cpp \
    ScalingConfig.cpp \
    BrightnessConfig.cpp \
    BlurConfig.cpp \
    ContrastConfig.cpp \
    MosaicConfig.cpp \
    Image.cpp \
    ImageEdit.cpp \
    Histogram.cpp \
    Constants.cpp \
    PS.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    GlobalInfo.h \
    PSStatus.h \
    tool.h \
    BasePSConfig.h \
    ScalingConfig.h \
    BrightnessConfig.h \
    BlurConfig.h \
    ContrastConfig.h \
    MosaicConfig.h \
    Image.h \
    ImageEdit.h \
    Histogram.h \
    Constants.h \
    PS.h
