#-------------------------------------------------
#
# Project created by QtCreator 2013-03-07T12:39:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = INDIClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    indiclientdlg.cpp \
    libindi-0.9.6/libs/indibase/basedevice.cpp \
    libindi-0.9.6/libs/indibase/baseclient.cpp \
    libindi-0.9.6/libs/lilxml.c \
    libindi-0.9.6/libs/indibase/indiproperty.cpp \
    libindi-0.9.6/libs/indicom.c \
    libindi-0.9.6/base64.c \
    mylineedit.cpp \
    indiswitch.cpp

HEADERS  += mainwindow.h \
    indiclientdlg.h \
    libindi-0.9.6/libs/indibase/basedevice.h \
    libindi-0.9.6/libs/indibase/baseclient.h \
    libindi-0.9.6/indiapi.h \
    libindi-0.9.6/indidevapi.h \
    libindi-0.9.6/libs/indibase/indibase.h \
    libindi-0.9.6/libs/lilxml.h \
    libindi-0.9.6/libs/indibase/indiproperty.h \
    libindi-0.9.6/libs/indicom.h \
    libindi-0.9.6/base64.h \
    config.h \
    mylineedit.h \
    indiswitch.h

FORMS    += mainwindow.ui

INCLUDEPATH += libindi-0.9.6 \ 
	libindi-0.9.6\libs

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../Qt/Qt5.0.1/Tools/MinGW/i686-w64-mingw32/release/ -llibwsock32
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../Qt/Qt5.0.1/Tools/MinGW/i686-w64-mingw32/debug/ -llibwsock32
else:unix: LIBS += -L$$PWD/../../../../../Qt/Qt5.0.1/Tools/MinGW/i686-w64-mingw32/ -l/usr/lib64/libz.a

INCLUDEPATH += $$PWD/../../../../../Qt/Qt5.0.1/Tools/MinGW/i686-w64-mingw32/include
DEPENDPATH += $$PWD/../../../../../Qt/Qt5.0.1/Tools/MinGW/i686-w64-mingw32/include
