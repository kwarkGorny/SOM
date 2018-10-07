TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system

SOURCES += main.cpp

HEADERS += \
    SOM.h
