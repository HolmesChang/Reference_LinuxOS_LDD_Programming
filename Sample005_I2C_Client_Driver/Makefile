obj-m := Sample005_I2C_Client_Driver_I2C_Board_Info.o
obj-m += Sample005_I2C_Client_Driver_I2C_Platform_Driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
