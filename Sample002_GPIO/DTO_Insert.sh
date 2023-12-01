#! /bin/bash

sudo mkdir /sys/kernel/config/device-tree/overlays/Sample002_GPIO_DTO

sudo cat Sample002_GPIO_DTO.dtbo > /sys/kernel/config/device-tree/overlays/Sample002_GPIO_DTO/dtbo
