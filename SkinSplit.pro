#-------------------------------------------------
#
# Project created by QtCreator 2014-11-28T23:07:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SkinSplit
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    facedetect.cpp \
    windowhelper.cpp \
    skinsplit.cpp

HEADERS  += mainwindow.h \
    facedetect.h \
    windowhelper.h \
    skinsplit.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons/icons.qrc

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv\
    gsl

}
