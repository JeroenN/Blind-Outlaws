TEMPLATE = app
CONFIG += console core qt
CONFIG -= app_bundle

SOURCES += \
    player.cpp \
    bullet.cpp \
    initializing.cpp \
    client.cpp \
    wall.cpp \
    bulletcode.cpp \
    main.cpp \
    server.cpp \
    tiles.cpp

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
LIBS += -LC:/SFML/lib -LC:/SFML/bin
LIBS += -lsfml-network

INCLUDEPATH += C:/SFML/include
DEPENDPATH += C:/SFML/include

HEADERS += \
    server.h \
    player.h \
    bullet.h \
    initializing.h \
    client.h \
    wall.h \
    bulletcode.h

RESOURCES += \
    blind-outlaws.qrc

