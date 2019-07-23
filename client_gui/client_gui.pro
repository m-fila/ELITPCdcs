#-------------------------------------------------
#
# Project created by QtCreator 2019-07-12T16:12:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client_gui
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11 -ggdb3 -Wno-unused
LIBS += -L/usr/local/lib/ -lopen62541
INCLUDEPATH +=I/user/local/include -I.

SOURCES += main.cpp\
        src/mainwindow.cpp \
        src/opc_client.cpp \
        src/state.cpp \
    src/lvpsuwidget.cpp \
    src/opcQObject.cpp \
    src/lvcontroller.cpp \
    src/opc_controller.cpp \
    src/tpgwidget.cpp \
    src/tpgcontroller.cpp \
    src/hvpsuwidget.cpp \
    src/hvcontroller.cpp \
    src/kled.cpp

HEADERS  += include/mainwindow.h \
    ../common/DTcDataType.h \
    ../common/DTmDataType.h \
    include/opc_client.h \
    include/state.h \
    include/lvpsuwidget.h \
    include/opcQObject.h \
    include/lvcontroller.h \
    ../common/HMPDataType.h \
    include/opc_controller.h \
    include/tpgwidget.h \
    include/tpgcontroller.h \
    include/hvpsuwidget.h \
    include/hvcontroller.h \
    include/kled.h

FORMS    += forms/mainwindow.ui \
    forms/lvpsuwidget.ui \
    forms/hvpsuwidget.ui

RESOURCES += resources.qrc
