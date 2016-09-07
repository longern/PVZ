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
	plants/plant.cpp \
	zombies/zombie.cpp \
    plants/peashooter.cpp \
    client.cpp \
    animations.cpp \
    plants/snowpea.cpp \
    plants/sunflower.cpp \
    plants/wallnut.cpp \
    plants/plants.cpp \
    sound.cpp \
    zombies/zombies.cpp \
    zombies/basiczombie.cpp \
    gamelogic.cpp \
    zombies/bucketheadzombie.cpp \
    zombies/polevaultingzombie.cpp \
	plants/cherrybomb.cpp \
    plantcard.cpp \
    zombies/coneheadzombie.cpp \
    zombies/jackintheboxzombie.cpp \
    plants/repeater.cpp \
    replayinterface.cpp

HEADERS  += mainwindow.h \
    welcomeinterface.h \
    playinginterface.h \
	plants/plant.h \
	zombies/zombie.h \
    plants/peashooter.h \
    client.h \
    plants/plants.h \
    zombies/zombies.h \
    plants/snowpea.h \
    plants/sunflower.h \
    plants/wallnut.h \
    sound.h \
    zombies/basiczombie.h \
    gamelogic.h \
    zombies/bucketheadzombie.h \
    zombies/polevaultingzombie.h \
	plants/cherrybomb.h \
    plantcard.h \
    zombies/coneheadzombie.h \
    zombies/jackintheboxzombie.h \
    plants/repeater.h \
    replayinterface.h

FORMS    += \
    welcomeform.ui \
    playform.ui \
    plantcard.ui

RESOURCES += \
    image.qrc \
    audio.qrc

TRANSLATIONS += \
	zh_CN.ts
