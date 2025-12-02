# Makefile for Radeon GPU Test

KERNEL_SOURCE ?= /lib/modules/$(shell uname -r)/build

# Kernel module object definitions
MOD_NAME = radeon_gpu_test_driver
OBJ_FILES = $(MOD_NAME).o

# Targets for compiling driver and app
all: driver app

# Compile kernel module
driver: $(OBJ_FILES)
	$(CC) -o $(MOD_NAME).ko $(OBJ_FILES)

# Compile application
app: radeon_gpu_test_app.c
	$(CC) -o radeon_gpu_test_app radeon_gpu_test_app.c

# Load and unload module
load:
	/sbin/insmod $(MOD_NAME).ko

unload:
	/sbin/rmmod $(MOD_NAME)

# Clean targets
clean:
	rm -f $(MOD_NAME).ko radeon_gpu_test_app $(OBJ_FILES)