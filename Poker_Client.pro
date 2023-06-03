QT       += core gui multimedia websockets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

#QMAKE_CXXFLAGS += -flto -Wl,-allow-multiple-definition
#QMAKE_CXXFLAGS += -Wa,-mbig-obj

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
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
    Base/protokoll.cpp \
    Base/user.cpp \
    Client/ClickableLabel.cpp \
    Client/gamemodel.cpp \
    Client/launcher.cpp \
    Client/launcherdialog.cpp \
    Client/main.cpp \
    Client/mainwindow.cpp \
	Client/wss_client.cpp
	
HEADERS += \
    Base/Board.h \
    Base/Card.h \
    Base/DbQuery.hpp \
    Base/Deck.h \
    Base/Game.h \
    Base/Hand.h \
    Base/Helpers.hpp \
    Base/Player.h \
    Base/protokoll.hpp \
    Base/user.hpp \
    Client/ClickableLabel.h \
    Client/gamemodel.hpp \
    Client/launcher.hpp \
    Client/launcherdialog.hpp \
    Client/mainwindow.h \
    Client/wss_client.h

FORMS += \
    Client/launcherdialog.ui \
    Client/mainwindow.ui

INCLUDEPATH += c:/Users/Thomas/source/repos/websocketpp \
               "c:/Program Files/OpenSSL-Win64/include"
			   
#INCLUDEPATH += C:/libs/boost_1_75_0

#win32: LIBS += -lWSock32 \
#                -lws2_32 \
#                -L"c:/Program Files/OpenSSL-Win64/lib" -llibssl -llibcrypto -lopenssl \
#                -LC:/libs/boost_1_75_0/stage/lib -llibboost_thread-mgw8-mt-d-x64-1_75 -llibboost_system-mgw8-mt-d-x64-1_75

RESOURCES     = Client/Poker_Client.qrc

TRANSLATIONS += \
    Client/Poker_Client_de_DE.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

ANDROID_ABIS = arm64-v8a
