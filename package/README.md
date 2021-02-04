# Packaging

The project can be installed as a .deb package (only Debian-based distributions).  
This installation method addinationaly creates `edcs` user and `dcs-server.service` — `dcsServer` as a _daemon_  for `systemd`.

## Instalation

Proceed with general building [instructions](../README.md), then run the following:

``` bash
cmake -DCMAKE_INSTALL_PREFIX=/opt/soft/dcs ..
make package
# replace 1.7 according to the version built
sudo dpkg -i ELITPC_DCS-1.7-Linux.deb  
```

The instalation path for package can be controlled with `-DCMAKE_INSTALL_PREFIX=/your-path`.

### Removal
```bash
sudo dpkg -r ELITPC_DCS
```
##  Usage
### System service

Enable to start the `dcsServer` automatically at boot:
```bash
sudo systemctl enable dcs-server.service
```
Replace `enable` with `start` to start immediately, with `stop` to stop immediately, with `status` to check if the service is running.

### Configuration file

The configuration file used by the `dcs-sever.service` is installed in `~dcs\dcs.json`.  
You can edit the file directly with `sudo` or add your user to the `edcs` group:

```bash
sudo usermod -aG edcs username
```
### Client

Connect to the dcs server run by systemd:

```bash
/opt/soft/dcs/bin/dcsGUI ~dcs/dcs.json
# or /your-path/bin/dcsGUI
```
