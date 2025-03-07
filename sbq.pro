#-------------------------------------------------
#
# Project created by QtCreator 2019-01-31T11:38:01
#
#-------------------------------------------------

QT       += core xml core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sbq
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

VCGLIB_PATH = $$PWD/vcglib/
LIBQGLVIEWER_PATH= $$PWD/QGLViewer

CONFIG += c++11

INCLUDEPATH += $$VCGLIB_PATH/ $$LIBQGLVIEWER_PATH

LIBS += -L$$LIBQGLVIEWER_PATH -lQGLViewer-qt5 -lGLEW  -lGLU -lGL -fopenmp
#LIBS += -lQGLViewer-qt5

# Subprojects
SUBDIRS = \
        $$LIBQGLVIEWER_PATH

SOURCES += \
        main.cpp \
    vcglib/wrap/ply/plylib.cpp \
    mainwindow.cpp \
    application.cpp \
    viewer.cpp

HEADERS += \
        mainwindow.h \
    application.h \
    viewer.h \
    meshtypes.h \
    utils.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
