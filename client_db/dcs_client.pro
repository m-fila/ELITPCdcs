TEMPLATE = app
CONFIG += console c++11

QMAKE_CXXFLAGS += -Wall -Wno-unused
QMAKE_CXXFLAGS += -std=c++11 -ggdb3
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -L/usr/local/lib/ -lopen62541
LIBS += -lsqlite3
INCLUDEPATH +=I/user/local/include -I.


SOURCES += main.cpp \
    src/database.cpp \
    src/opc_client.cpp \
    src/abstract_variable.cpp \
    src/hmp_variable.cpp \
    src/dt1415_variable.cpp

HEADERS += \
    ../common/HMPDataType.h \
    include/database.h \
    include/opc_client.h \
    include/abstract_variable.h \
    include/template_variable.h \
    include/hmp_variable.h \
    include/dt1415_variable.h
