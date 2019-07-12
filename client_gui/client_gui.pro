#-------------------------------------------------
#
# Project created by QtCreator 2019-07-12T16:12:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client_gui
TEMPLATE = app
LIBS += -L/usr/local/lib/ -lopen62541
INCLUDEPATH +=I/user/local/include -I.

SOURCES += main.cpp\
        src/mainwindow.cpp \
    include/opc_client.cpp

HEADERS  += include/mainwindow.h \
    include/opc_client.h

FORMS    += forms/mainwindow.ui
