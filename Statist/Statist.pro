######################################################################
# Automatically generated by qmake (3.1) Sat Jan 2 21:48:25 2021
######################################################################

TEMPLATE = app
TARGET = Statist
INCLUDEPATH += .

QT += sql websockets

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += ../Base/Board.h \
           ../Base/Card.h \
           ../Base/Deck.h \
           ../Base/Game.h \
           ../Base/Hand.h \
           ../Base/Player.h \
           ../Base/user.hpp \
	   ../Base/Helpers.hpp \
	   ../Base/DbQuery.hpp
SOURCES += src/main.cpp \
	   ../Base/Board.cpp \
           ../Base/Card.cpp \
           ../Base/Deck.cpp \
           ../Base/Game.cpp \
           ../Base/Hand.cpp \
           ../Base/Player.cpp \
           ../Base/user.cpp \
	   ../Base/Helpers.cpp \
	   ../Base/DbQuery.cpp
