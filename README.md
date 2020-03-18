# ![ELITPCS dcs](docs/img/dcs_logo.png "ELITPC dcs")

Detector Control System for ELITPC detector using OPC UA protocol.
Provides server, operational gui client and database client. Server is
compatible with other clients such as UaExpert by Unified Automation.

## Getting Started

### Prerequisites
C++11 compiler and cmake>3.1

This project requires open62541 (open source C implementation of OPC UA)
build with enabled methodcalls, subscriptions (enabled by default) and full namespace 0. You may change `-DCMAKE_INSTALL_PREFIX` for custom installation (in that case you should also edit env.sh with same path):
```
git clone https://github.com/open62541/open62541
cd open62541
git checkout v1.0.1
git submodule update --init --recursive
mkdir build && cd build
cmake -DUA_NAMESPACE_ZERO=FULL  -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=/opt/soft/open62541 ..
make
sudo make install
```
Furthermore the GUI clients requires Qt5 :
```
sudo apt-get install qt5-default
```
### Building
```
. env.sh
mkdir build && cd build
cmake ..
make
make install
```

CMake flags `-DBUILD_DB` `-DBUILD_GUI` `-DBUILD_SERVER`  can be switched to `OFF` to disable building some parts of the project.
`-DCMAKE_INSTALL_PREFIX=your_path` can be used to specify custom installation path.


Upon successful build the resulting executables `dscServer`, `dcsGui` and `dcsDb` will be available:
* dcsServer - provides OPC server and device controllers. Should be run on a computer with access to devices.
* dcsGui - GUI client. Provides graphic interface for utilies exposed to user by server such as monitoring and manipulating devices.
* dcsDb - data logger client . Connects to server and loggs states and measurements into local SQLite database.

## Usage

At startup server and clients look for dcs.json file with list of
devices used in experimental setup and address of a server. So far known devices are:
* HMP2020 - 2 channel lv psu
* HMP4040 - 4 channel lv psu
* DT1415ET - hv psu
* TPG362 - vacuum gauge
* MKS910 - vacuum gauge
* PiWeather - custom Raspberry Pi powered weather station

A path to config file should be given in first argument for server or client application. Otherwise a shared config can be placed at `${HOME}/.dcs/dcs.json`.
You may consider installing `sqlitebrowser` for reading database files:
```
sudo apt-get install sqlitebrowser
```

### Adding new devices
The project is split into two catalogs COREdcs with framework and ELITPCdcs implementing handling specific devices. Ideally you shouldn't have to edit anything in COREdcs.
#### Server
* `bin/main.cpp` create specific device controller if found in config file
* `${device}` hardware communication with device
* `${device}controller`  device specific methods and update routines for
variables of OPC object
#### Database client
* `bin/main.cpp` create specific device controller if found in config file
* `${device}variable` logging specific parameters of variables provided by server
#### GUI client
* `mainwindow.cpp` create specific device controller if found in config file
* `${device}controller` slots for calling device specific methods
* `${device}widget` device specific gui
### Roadmap
- [ ] new devices (MKS 946) 
- [ ] events
- [ ] historizing
- [ ] more efficient server side
## Authors
* __Mateusz Fila__ basing on ELITPCdcsPanel by __Marcin Zaremba__