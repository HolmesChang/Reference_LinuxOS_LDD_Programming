obj-m := Sample006_SPI_Client_Driver_SPI_Board_Info.o
obj-m += Sample006_SPI_Client_Driver_SPI_Platform_Driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
