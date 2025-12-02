# Makefile for Radeon GPU Test

# Compiler
CC = gcc

# Kernel module name
KMOD = radeon_gpu

# User space application name
APP = test_radeon

# Sources
KMOD_SRC = radeon_gpu.c
APP_SRC = test_radeon.c

# Objects
KMOD_OBJ = $(KMOD_SRC:.c=.o)
APP_OBJ = $(APP_SRC:.c=.o)

# Default build target
all: $(KMOD) $(APP)

# Build kernel module
$(KMOD): $(KMOD_OBJ)
\t$(CC) -shared -o $@ $^

# Build user-space application
$(APP): $(APP_OBJ)
\t$(CC) -o $@ $^

# Install kernel module
install: $(KMOD)
\t@echo Installing kernel module...
\t/sbin/insmod $(KMOD)

# Load kernel module
load:
\t@echo Loading kernel module...
\t/sbin/modprobe $(KMOD)

# Unload kernel module
unload:
\t@echo Unloading kernel module...
\t/sbin/rmmod $(KMOD)

# Run tests
run_tests:
\t@echo Running tests...
\t./$(APP)

# Clean up
clean:
\trm -f $(KMOD) $(APP) $(KMOD_OBJ) $(APP_OBJ)

.PHONY: all install load unload run_tests clean
