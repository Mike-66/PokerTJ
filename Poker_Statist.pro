QT -= gui
QT += sql websockets

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Base/Board.cpp \
    Base/Card.cpp \
    Base/DbQuery.cpp \
    Base/Deck.cpp \
    Base/Game.cpp \
    Base/Hand.cpp \
    Base/Helpers.cpp \
    Base/Player.cpp \
    Base/user.cpp \
    Statist/src/main.cpp 

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Base/Board.h \
    Base/Card.h \
    Base/DbQuery.hpp \
    Base/Deck.h \
    Base/Game.h \
    Base/Hand.h \
    Base/Helpers.hpp \
    Base/Player.h \
    Base/user.hpp 
