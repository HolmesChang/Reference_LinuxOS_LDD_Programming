#! /bin/bash

sudo mkdir /sys/kernel/config/device-tree/overlays/Sample006_SPI_DTO

sudo cat Sample006_SPI_DTO.dtbo > /sys/kernel/config/device-tree/overlays/Sample006_SPI_DTO/dtbo
