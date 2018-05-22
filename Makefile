obj-m += test-module.o
all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -C ~/android/linux/ M=$(PWD) modules
clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -C ~/android/linux/ M=$(PWD) clean