KERNELDIR := /home/jhs/luckfox-pico/sysdrv/source/kernel
KERNELOBJ := /home/jhs/luckfox-pico/sysdrv/source/objs_kernel
CROSS_COMPILE := arm-rockchip830-linux-uclibcgnueabihf-
CURRENT_PATH := $(shell pwd)

obj-m := ssd130x.o ssd130x_i2c.o

all: app

# kernel_modules:
# 	$(MAKE) O=$(KERNELOBJ) -C $(KERNELDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) M=$(CURRENT_PATH) modules

app:
	$(CROSS_COMPILE)gcc -o oled_main oled_main.c

clean:
# 	$(MAKE) -C $(KERNELDIR) ARCH=arm M=$(CURRENT_PATH) clean
	rm -f oled_main