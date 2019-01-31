#-------------------------------------------------
#
# Project created by QtCreator 2019-01-01T04:55:12
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Voicetainment
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    voiceinput.cpp

HEADERS += \
        mainwindow.h \
    voiceinput.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += sphinx/sphinxbase/include/ sphinx/pocketsphinx/include/ \
				sphinx/pocketsphinx/src/libpocketsphinx/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -L$$PWD/sphinx/pocketsphinx/lib/ -lpocketsphinx
unix|win32: LIBS += -L$$PWD/sphinx/sphinxbase/lib/ -lsphinxbase

INCLUDEPATH += $$PWD/sphinx/pocketsphinx/include
DEPENDPATH += $$PWD/sphinx/pocketsphinx/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/sphinx/pocketsphinx/lib/pocketsphinx.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/sphinx/pocketsphinx/lib/libpocketsphinx.a
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/sphinx/sphinxbase/lib/sphinxbase.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/sphinx/sphinxbase/lib/libsphinxbase.a
