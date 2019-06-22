TEMPLATE = app
CONFIG += console c++11

QMAKE_CXXFLAGS += -Wall -Wno-unused
QMAKE_CXXFLAGS += -std=c++11 -ggdb3
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -L/usr/local/lib/ -lopen62541
LIBS += -lsqlite3
INCLUDEPATH +=I/user/local/include


SOURCES += \
        main.cpp \
        src/db.cpp \
        src/opc_client.cpp \
    src/abstract_variable.cpp \
    src/hmp_variable.cpp

HEADERS += \
    src/HMPDataType.h \
    src/db.h \
    src/opc_client.h \
    src/abstract_variable.h \
    src/template_variable.h \
    src/hmp_variable.h
