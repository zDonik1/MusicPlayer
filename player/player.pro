TEMPLATE = lib
TARGET = player
CONFIG += dll
QT += core androidextras multimedia sql

SOURCES += \
    databasemanager.cpp \
    dirdao.cpp \
    musicdao.cpp \
    player.cpp \
    playerbinder.cpp \
    playerservice.cpp

HEADERS += \
    abstractdao.h \
    common/messagetype.h \
    databasemanager.h \
    dirdao.h \
    musicdao.h \
    playerbinder.h \
    playerservice.h

RESOURCES += \
    resources.qrc

INCLUDEPATH += ../common
