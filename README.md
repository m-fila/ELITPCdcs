# ELITPC DCS

Detector Control System for ELITPC detector using OPC UA protocol.
Provides server, operational gui client and database client. Server is
compatible with other clients such as UAExpert by Unified Automation.

## Getting Started

### Prerequisites
C++11 compiler and cmake>3.1

This project requires open62541 (open source C implementation of OPC UA)
build with enabled methodcalls, subscriptions and full namespace 0:

```
git clone https://github.com/open62541/open62541
mkdir open62541/build && cd open62541/build
git submodule update --init --recursive
cmake -UA_ENABLE_METHODCALLS=ON -DUA_ENABLE_SUBSCRIPTIONS=ON
-DUA_NAMESPACE_ZERO=FULL ..
```
Furthermore database and GUI clients requires respectively sqlite3 and Qt5 :
```
sudo apt-get install sqlite3
sudo apt-get install qt5-default
```

### Building
```
mkdir build && cd build
cmake ..
```

`-DBUILD_DB` `-DBUILD_GUI` `-DBUILD_SERVER`  can be switched to `OFF` to
disable building some parts of the project.




## Monitoring devices

At startup server and clients look for dcs.config file with list of
devices used in experimental setup. So far known devices are:
* HMP2020 - 2 channel lv psu
* HMP4040 -4 channel lv psu
* DT1415ET - hv psu


Example line from config file `DEVICETYPE: UNIQUE_ID OPTIONAL_ADDRESS
OPTIONAL_PORT`:
```
HMP2020: HMP2 192.168.168.20 5025
```


### Adding new devices
#### Server
`main.cpp` create specific device controller if found in config file
`hw/*` hardware communication with device
`opc/*controller`  device specific methods and update routines for
variables of OPC object


#### Database client
`main.cpp` create specific device controller if found in config file
`*variable` logging specific parameters of variables provided by server
#### GUI client
`mainwindow.cpp` create specific device controller if found in config file
`*controller` slots to cal device specific methods
`*widget` device specific gui
## Authors
* __Mateusz Fila__ basing on ELITPCdcsPanel by __Marcin Zaremba__