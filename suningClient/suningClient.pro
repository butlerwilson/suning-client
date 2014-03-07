#-------------------------------------------------
#
# Project created by QtCreator 2013-12-18T22:38:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = suningClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    product.cpp \
    isearch.cpp

HEADERS  += mainwindow.h \
    syscntl.h \
    product.h \
    isearch.h

FORMS    += mainwindow.ui
