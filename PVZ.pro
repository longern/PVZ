#-------------------------------------------------
#
# Project created by QtCreator 2016-08-25T14:45:35
#
#-------------------------------------------------

QT       += core gui multimedia network
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PVZ
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    welcomeinterface.cpp \
    playinginterface.cpp \
    gameoverinterface.cpp \
	plants/plant.cpp \
	zombies/zombie.cpp \
    plants/peashooter.cpp \
    client.cpp \
    animations.cpp \
    plants/snowpea.cpp \
    plants/sunflower.cpp \
    plants/wallnut.cpp \
    plants/plants.cpp

HEADERS  += mainwindow.h \
    welcomeinterface.h \
    playinginterface.h \
    gameoverinterface.h \
	plants/plant.h \
	zombies/zombie.h \
    plants/peashooter.h \
    client.h \
    plants/plants.h \
    zombies/zombies.h \
    plants/snowpea.h \
    plants/sunflower.h \
    plants/wallnut.h

FORMS    += \
    welcomeform.ui \
    playform.ui

RESOURCES += \
    image.qrc

TRANSLATIONS += \
	zh_CN.ts
