MY_CFLAGS += -g -DDEBUG
ccflags-y += ${MY_CFLAGS}
CC += ${MY_CFLAGS}
TARGET_MODULE:=helloworld

# If we running by kernel building system
ifneq ($(KERNELRELEASE),)
	$(TARGET_MODULE)-objs := main.o device_file.o
	obj-m := $(TARGET_MODULE).o

# If we are running without kernel build system
else
	BUILDSYSTEM_DIR:=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)


all :
# run kernel build system to make module
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) modules

debug:
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) modules
	EXTRA_CFLAGS="$(MY_CFLAGS)"

clean:
# run kernel build system to cleanup in current directory
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) clean

load:
	sudo insmod ./$(TARGET_MODULE).ko && \
	sudo cp $(TARGET_MODULE).o /tmp/ && \
	sudo cp /sys/module/$(TARGET_MODULE)/sections/.text /tmp/$(TARGET_MODULE)_text && \
	sudo chmod a+r /tmp/$(TARGET_MODULE)_text

unload:
	sudo rmmod $(TARGET_MODULE)

endif
