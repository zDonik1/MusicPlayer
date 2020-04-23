TEMPLATE = lib
TARGET = player
CONFIG += dll
QT += core androidextras multimedia sql

SOURCES += \
    databasemanager.cpp \
    musicdao.cpp \
    player.cpp \
    playerbinder.cpp \
    playerservice.cpp

HEADERS += \
    databasemanager.h \
    musicdao.h \
    playerbinder.h \
    playerservice.h

RESOURCES += \
    resources.qrc
