Introduction
============
ELITPC dcs is an `open source <https://dracula.hep.fuw.edu.pl:8888/mfila/dcs>`_ Detector Control System for ELITPC detector.

OPC UA
------
OPC Unified Architecture is a machine to machine communication protocol for industrial automation developed by the OPC Foundation
and specified in the standard IEC 62541. Service oriented architecture.
Used as middleware layer in ELITPC dcs. 

nodes and references between them.

open source C99 implementation are implemented as in the spirit of object oriented programing.

open62541 seems to be the OPC UA implementation of choice for other C/C++ projects:

  - well documented,
  -  well tested,
  - actively supported,
  - has active community (github, stack overflow, google group).


Features
------------------

- Industrial standard communication protocol
- Server application
  
  - monitors state of devices,
  - stores states in database,
  - controls devices with methods,
  - sends notifications with events,
  - adds preconfigured profiles to any device.

- Client application

  - human machine interface,
  - graphical interface based on QtWidgets library. 

- Customizable with config files


Alternatives
------------
The aim of this project is to provide simple and easy to manage detector control system with minimal overhead. 
More powerful but also more complex frameworks used in huge high energy physics experiments are: 

- `Tango <https://www.tango-controls.org/>`_ - SLAC, ORNL, FAIR, ... 
- `EPICS <https://epics-controls.org/>`_  - DESY, SERF, SOLARIS, ...


Acknowledgment
--------------
Scientific work supported by the Polish Ministry of Science and Higher Education from the funds for years 2019-2021 
dedicated to implement the international co-funded project no. 4087/ELI-NP/2018/0, by University of Connecticut under
the Collaborative Research Contract no. UConn-LNS_UW/7/2018 and by the National Science Centre, Poland, under Contract
no. UMO-2019/33/B/ST2/02176.
