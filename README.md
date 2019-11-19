# ELITPC DCS

Detector Control System for ELITPC detector using OPC UA protocol.
Provides server, operational gui client and database client. Server is
compatible with other clients such as UAExpert by Unified Automation.

## Getting Started

### Prerequisites
C++11 compiler and cmake>3.1

This project requires open62541 (open source C implementation of OPC UA)
build with enabled methodcalls, subscriptions (enabled by default) and full namespace 0. You may change `-DCMAKE_INSTALL_PREFIX` for custom installation:
```
git clone https://github.com/open62541/open62541
mkdir open62541/build && cd open62541/build
git submodule update --init --recursive
cmake -DUA_NAMESPACE_ZERO=FULL  -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=/opt/soft ..
make
sudo make install
```
Furthermore database and GUI clients requires respectively sqlite3 and Qt5 :
```
sudo apt-get install libsqlite3-dev
sudo apt-get install qt5-default
```
### Building
```
mkdir build && cd build
cmake ..
```

`-DBUILD_DB` `-DBUILD_GUI` `-DBUILD_SERVER`  can be switched to `OFF` to disable building some parts of the project. In case of custom installation path  line `set(CMAKE_PREFIX_PATH /opt/soft)` in `dcs/CMakeLists.txt` should be edited.
Upon successful build the resulting executables `dscServer`, `dcsGui` and `dcsDb` will be available in `build/bin` directory.


## Monitoring devices

At startup server and clients look for dcs.config file with list of
devices used in experimental setup and address of a server. So far known devices are:
* HMP2020 - 2 channel lv psu
* HMP4040 - 4 channel lv psu
* DT1415ET - hv psu
* TPG362 - vacuum gauge
* PiWeather - custom Raspberry Pi powered weather station

Example line from config file `DEVICETYPE: UNIQUE_ID OPTIONAL_ADDRESS
OPTIONAL_PORT`:
```
HMP2020: HMP2 192.168.168.20 5025
```
Exampline line conntaining server address `SERVER: dumy ADDRESS PORT`:
```
SERVER: serv1 192.168.168.2 6669
```
A path to config file should be given in first argument for server or client application. Otherwise a shared config can be placed at `${HOME}/.config/dcs.config`.

You may consider installing `sqlitebrowser` for reading database files:
```
sudo apt-get install sqlitebrowser
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
`*controller` slots for calling device specific methods
`*widget` device specific gui
## Authors
* __Mateusz Fila__ basing on ELITPCdcsPanel by __Marcin Zaremba__