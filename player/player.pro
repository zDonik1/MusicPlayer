TEMPLATE = lib
TARGET = player
CONFIG += dll
QT += core androidextras multimedia

SOURCES += \
    main.cpp \
    player.cpp \
    playerbinder.cpp \
    playerservice.cpp

HEADERS += \
    player.h \
    playerbinder.h \
    playerservice.h

RESOURCES += \
    resources.qrc

INCLUDEPATH += ../common
