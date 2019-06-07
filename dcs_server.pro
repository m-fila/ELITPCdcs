TEMPLATE = app
CONFIG += console c++11

QMAKE_CXXFLAGS += -Wall -Wno-unused
QMAKE_CXXFLAGS += -std=c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += /usr/local/lib/libopen62541.a
INCLUDEPATH +=/user/local/include

SOURCES += src/main.cpp \
    src/hw/HMP2020.cpp \
    src/utl/ConnectionStream.cpp \
    src/utl/TCPConnector.cpp \
    src/utl/TCPStream.cpp \
    src/utl/GenericDevice.cpp \
    src/hw/DT1415ET.cpp \
    src/hw/TPG362.cpp \
    src/DeviceController.cpp \
 #   src/utl/lvpsucontroller.cpp \
    src/opc/opcserver.cpp \
    src/opc/hmpcontroller.cpp \
    src/opc/opc_monitor.cpp



HEADERS  +=  src/ConnectionParameters.h \
    src/hw/HMP2020.h \
    src/utl/TCPStream.h \
    src/utl/TCPConnector.h \
    src/utl/ConnectionStream.h \
    src/utl/GenericDevice.h \
    src/hw/DT1415ET.h \
    src/hw/TPG362.h \
    src/DeviceController.h \
 #   src/utl/lvpsucontroller.h \
    src/opc/opcserver.h \
    src/opc/hmpcontroller.h \
    src/opc/HMPDataType.h \
    src/opc/opc_monitor.h \
    src/opc/opc_controller.h \
    src/opc/opc_controller.hxx


