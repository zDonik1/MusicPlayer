TEMPLATE = lib
TARGET = player
CONFIG += dll
QT += core androidextras multimedia sql

SOURCES += \
    player.cpp \
    playerbinder.cpp \
    playerservice.cpp

HEADERS += \
    playerbinder.h \
    playerservice.h

RESOURCES += \
    resources.qrc

INCLUDEPATH += ../common
