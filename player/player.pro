TEMPLATE = lib
TARGET = player
CONFIG += dll
QT += core androidextras multimedia

SOURCES += \
    player.cpp \
    playerbinder.cpp \
    playerservice.cpp

HEADERS += \
    playerbinder.h \
    playerservice.h

RESOURCES += \
    resources.qrc
