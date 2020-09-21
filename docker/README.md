# ELITPC dcs docker
## Base image
Base image `mfila/open62541` is available at [Docker Hub](https://hub.docker.com/r/mfila/open62541). Its Dockerfile is also available at [Git Hub](https://github.com/m-fila/dcs-docker-base). You don't have to go there, the image will pull automatically if you decide to play with `ELITPCdcs` image.

## ELITPCdcs image

This image include ELTIPC dcs server installed in `/usr/local/` . The `Dockerfile` is used in official test builds.

## Build locally

To build the `ELITPCdcs` image locally use:

```
git clone ssh://git@dracula.hep.fuw.edu.pl:8822/mfila/dcs.git
cd dcs
sudo docker build -f docker/Dockerfile .
```
