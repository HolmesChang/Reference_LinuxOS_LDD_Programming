#! /bin/bash

sudo mkdir /sys/kernel/config/device-tree/overlays/Sample005_I2C_DTO

sudo cat Sample005_I2C_DTO.dtbo > /sys/kernel/config/device-tree/overlays/Sample005_I2C_DTO/dtbo
