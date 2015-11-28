#-------------------------------------------------
#
# Project created by QtCreator 2015-11-26T16:18:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ROIEditor
TEMPLATE = app
ICON = icon.icns

SOURCES += main.cpp\
        mainwindow.cpp \
    ImageBay.cpp

HEADERS  += mainwindow.h \
    ImageBay.h

FORMS    += mainwindow.ui
