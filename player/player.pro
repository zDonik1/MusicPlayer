TEMPLATE = lib
TARGET = player
CONFIG += dll
QT += core androidextras

SOURCES += \
    player.cpp \
    playerbinder.cpp \
    playerservice.cpp

HEADERS += \
    playerbinder.h \
    playerservice.h
