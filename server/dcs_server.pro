TEMPLATE = app
CONFIG += console c++11

QMAKE_CXXFLAGS += -Wall -Wno-unused
QMAKE_CXXFLAGS += -std=c++11 -ggdb3
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -L/usr/local/lib/ -lopen62541
INCLUDEPATH +=/user/local/include -I.

SOURCES += main.cpp \
    src/hw/HMP2020.cpp \
    src/utl/ConnectionStream.cpp \
    src/utl/TCPConnector.cpp \
    src/utl/TCPStream.cpp \
    src/utl/GenericDevice.cpp \
    src/hw/DT1415ET.cpp \
    src/hw/TPG362.cpp \
    src/DeviceController.cpp \
    src/opc/hmpcontroller.cpp \
    src/opc/opc_monitor.cpp \
    src/opc/opc_state.cpp \
    src/opc/opc_object.cpp \
    src/opc/opc_server.cpp



HEADERS  +=  \
    include/ConnectionParameters.h \
    include/hw/HMP2020.h \
    include/utl/TCPStream.h \
    include/utl/TCPConnector.h \
    include/utl/ConnectionStream.h \
    include/utl/GenericDevice.h \
    include/hw/DT1415ET.h \
    include/hw/TPG362.h \
    include/DeviceController.h \
    include/opc/hmpcontroller.h \
    ../common/HMPDataType.h \
    include/opc/opc_monitor.h \
    include/opc/opc_controller.h \
    include/opc/opc_controller.hxx \
    include/opc/opc_state.h \
    include/opc/opc_object.h \
    include/opc/opc_server.h


