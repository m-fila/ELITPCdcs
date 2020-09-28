# ![ELITPCS dcs](docs/icons/dcs_logo.png "ELITPC dcs")
Detector Control System for ELITPC detector using OPC UA protocol.
Provides server, operational gui client and database client. Server is
compatible with other OPC UA clients such as UaExpert by Unified Automation.

## Getting Started

### Prerequisites
C++11 compiler and cmake>3.1

This project requires open62541 (open source C implementation of OPC UA) version 1.1 or newer
build with enabled methodcalls, subscriptions (enabled by default), events, historizing, multithread-safety and full namespace 0. You may change `-DCMAKE_INSTALL_PREFIX` for custom installation (in that case you should also edit env.sh with the same path):
```
git clone https://github.com/open62541/open62541
cd open62541
git checkout v1.1
git submodule update --init --recursive
mkdir build && cd build
cmake -DUA_NAMESPACE_ZERO=FULL  -DBUILD_SHARED_LIBS=ON -DUA_ENABLE_HISTORIZING=ON -DUA_ENABLE_SUBSCRIPTIONS_EVENTS=ON -DUA_MULTITHREADING=150 -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=/opt/soft/open62541 ..
make
sudo make install
```
Furthermore the GUI clients requires Qt5 :
```
sudo apt-get install qt5-default
```
### Building
```
git clone --recursive ssh://git@dracula.hep.fuw.edu.pl:8822/mfila/dcs.git
. env.sh
mkdir build && cd build
cmake ..
make
make install
```

CMake flags `-DBUILD_DB` `-DBUILD_GUI` `-DBUILD_SERVER`  can be switched to `OFF` to disable building some parts of the project.
`cmake -DCMAKE_INSTALL_PREFIX=your_path ..` can be used to specify custom installation path.


Upon successful build the resulting executables `dscServer`, `dcsGui` and `dcsDb` will be available:
* dcsServer - provides OPC server and device controllers. Should be run on a computer with access to devices.
* dcsGui - GUI client. Provides graphic interface for operator user of services exposed by server such as monitoring and manipulating devices.
* dcsDb - data logger client . Connects to server and loggs states and measurements into local SQLite database (obsolete if you use historizing on server)

## Usage
* to run server:
  ```
  ./dcsServer (path_to_config_file)
  ```
* to run gui client:
  ```
  ./dcsGUI (path_to_config_file)
  ```
* to run database client:
    ```
  ./dcsDB (path_to_config_file)
  ```

At startup server and clients look for configuration file with list of
devices used in experimental setup and address of a server. So far known devices are:
* HMP2020 - 2 channel lv psu
* HMP4040 - 4 channel lv psu
* DT1415ET - 8 channel hv psu
* DT1740ET - 4 channel hv psu
* N1471 - 4 channel NIM hv psu
* TPG362 - vacuum gauge
* MKS910 - vacuum gauge
* PiWeather - custom Raspberry Pi powered weather station

A path to config file should be given in first argument for server or client application. Otherwise an enviromental variable will be looked for.

#### Enviromental variables
During initialisation dcsServer looks for enviromental variables. 
* `${DCS_CONFIG_FILE}` - path to server configuration (currently json) file. Default value is `${HOME}/.dcs/dcs.json`.First argument given to `dcsServer` has higher priority than this enviromental variable 
* `${DCS_PROFILE_DIR}` - path to directory with device profiles. Default value is `${HOME}/.dcs/`. Config field `profileDir` has higher priority than this enviromental variable 

#### Adding new devices (old)
The project is split into two catalogs COREdcs with framework and ELITPCdcs implementing specific devices. Ideally you shouldn't have to edit anything in COREdcs.
#### Server
* `bin/main.cpp` create specific device controller if found in config file
* `${device}` hardware communication with device
* `${device}controller`  device specific methods and update routines for variables of OPC object
#### GUI client
* `mainwindow.cpp` create specific device controller if found in config file
* `${device}controller` slots for calling device specific methods
* `${device}widget` device specific gui
### Roadmap
- [ ] new devices (MKS 946) 
- [ ] interlock support 
- [x] server events
- [x] historizing
- [x] refactorized server
- [x] server device register 
- [x] server device profiles
- [ ] refactorized client
- [ ] client events, profiles display
## License
This work is distributed under [MIT license](LICENSE).

`deps/` are distributed under their own licenses (see information in header files)
## Authors
* __Mateusz Fila__ basing on ELITPCdcsPanel by __Marcin Zaremba__
