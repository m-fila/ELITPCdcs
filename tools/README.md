# Tools

## tcp-server

Dummy tcp server for debuging.
### Prerequsites:
Python3
### Usage
```
tcp-server.py
```
## dcsDbDump
Python tool for dumping influxdb database records to .csv files. This script will be installed in `bin/dcsDbDump` with `make install`.
### Prerequsites:
Python3 and it's packages influxdb 1.x client and pandas

### Examples
- get help:
    ```
    dcsDbDump -h
    ```
- list all measurements from *chicken* database servered at *localhost:8086* (same as default):
    ```
    dcsDbDump -db chicken -l -ip localhost -p 8086
    ```
- get all measurements from *elitpc* database from last day
    ```
    dcsDbDump -db elitpc -et now -tl 1d
    ```
- get measurement *GEM-HV.measurements* from *elitpc* database collected during 2 hours from 2021-01-21T14:34:21.749Z (UTC)
    ```
    dcsDbDump -db elitpc -m GEM-HV.measurements -st 2021-01-21T14:34:21.749Z -tl 2h
    ```
    or from 2021-01-21T15:34:21.749+01:00 (same time but in EST)
    ```
    dcsDbDump -db elitpc -m GEM-HV.measurements -st 2021-01-21T15:34:21.749+01:00 -tl 2h
    ```
- get measurements *PM3.measurements* and *PID_Controller.measurements* from *elitpc* database between timestamps 1611234905 and 1611253360
    ```
    dcsDbDump -db elitpc -m PM3.measurements PID_Controller.measurements -st 1611234905 -et 1611253360
    ```