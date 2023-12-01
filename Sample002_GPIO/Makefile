obj-m := Sample002_GPIO.o
obj-m += Sample002_GPIO_Platform_Driver.o
obj-m += Sample002_GPIO_Platform_Device.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
