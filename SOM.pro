TEMPLATE = app
CONFIG += console c++1z
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system

QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O2 -DNDEBUG -fno-exceptions -flto -ffunction-sections -fdata-sections -finline-limit=150 -march=native -ffast-math

SOURCES += main.cpp

HEADERS += \
    SOM.h
