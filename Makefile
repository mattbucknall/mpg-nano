#
# MPG-Nano Firmware makefile.
#

# Arduino Nano serial port
ARDUINO_SERIAL = /dev/ttyUSB0

# C source files
C_SRC = \
 app-encoder.c \
 app-io.c \
 app-serial.c \
 app-switch.c \
 main.c

# Assembly source files
AS_SRC =
 
# output name (no extension)
OUTPUT = mpg-nano

# output format (binary, srec or ihex)
FORMAT = binary

# optimization level (0, 1, 2, 3 or s)
OPTIMIZATION_LEVEL = s

# target microcontroller
MCU = atmega328p

# target microcontroller clock frequency (Hz)
MCU_FREQ = 16000000

# programmer flags (for fuse, eeprom and flash programming)
PROG_COMMON_FLAGS = -c arduino -P $(ARDUINO_SERIAL)
PROG_COMMON_FLAGS += -p m328p

# toolchain prefix
TOOLCHAIN_PREFIX = avr-

# tools
export AR = $(TOOLCHAIN_PREFIX)ar
export AS = $(TOOLCHAIN_PREFIX)as
export CC = $(TOOLCHAIN_PREFIX)gcc
export COPY = cp
export LD = $(TOOLCHAIN_PREFIX)ld
export NM = $(TOOLCHAIN_PREFIX)nm
export OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
export OBJDUMP = $(TOOLCHAIN_PREFIX)objdump
export RANLIB = $(TOOLCHAIN_PREFIX)ranlib
export REMOVE = rm -f
export SHELL = /bin/sh
export SIZE = $(TOOLCHAIN_PREFIX)size
export PROG = avrdude
export HOST_CC = gcc

# Symbols for which to force linkage
FORCE_LINK =
 
# Include flags
INCLUDES = \
 -I.

# Common flags
COMMON = -mmcu=$(MCU)

# C compiler flags
CFLAGS = \
 $(COMMON) \
 $(INCLUDES) \
 -Wall -gdwarf-2 \
 -fsigned-char -fpack-struct -fshort-enums -funsigned-bitfields \
 -O$(OPTIMIZATION_LEVEL) \
 -D F_CPU=$(MCU_FREQ) \
 -std=c11

# compiler flags for generating dependency flags
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d

# Linker flags
LDFLAGS = \
 $(COMMON) \
 $(foreach symbol,$(FORCE_LINK),-u $(symbol)) 

# files to delete when cleaning
CLEAN_FILES= \
 $(OUTPUT).bin \
 $(OUTPUT).eep \
 $(OUTPUT).obj \
 $(OUTPUT).cof \
 $(OUTPUT).elf \
 $(OUTPUT).map \
 $(OUTPUT).a90 \
 $(OUTPUT).sym \
 $(OUTPUT).lnk \
 $(OUTPUT).lss \
 .dep/*

# rules

# C Object files
C_OBJ = $(C_SRC:.c=.o)

# Assembler object files
AS_OBJ = $(AS_SRC:.S=.o)

# All object files
OBJ = $(C_OBJ) $(AS_OBJ)

# files to delete when cleaning
CLEAN_FILES= \
 $(OUTPUT).elf \
 $(OUTPUT).bin \
 $(OUTPUT).sym \
 $(OBJ) \
 .dep/* \
 *.log

# rules
all: build

build: elf sym size

elf: $(OUTPUT).elf

bin: $(OUTPUT).bin

sym: $(OUTPUT).sym

%.elf: $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $^

%.bin: $(OUTPUT).elf
	$(OBJCOPY) -O $(FORMAT) $< $@

$(C_OBJ) : %.o : %.c
	$(CC) -c $(CFLAGS) $(GENDEPFLAGS) $< -o $@

$(AS_OBJ) : %.o : %.S
	$(CC) -c $(ASFLAGS) $(GENDEPFLAGS) $< -o $@

size: $(OUTPUT).elf
	$(SIZE) -B $(OUTPUT).elf

%.sym: $(OUTPUT).elf
	$(NM) -n $< > $@

clean:
	$(REMOVE) $(strip $(CLEAN_FILES))

program: $(OUTPUT).bin
	$(PROG) $(PROG_COMMON_FLAGS) $(PROG_FLASH_FLAGS) -u -U flash:w:$<

erase:
	$(PROG) $(PROG_COMMON_FLAGS) -e

reset:
	$(PROG) $(PROG_COMMON_FLAGS) -F noreset

# include dependency rules
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# phony targets
.PHONY: all build elf bin sym size clean program program_fuses program_all erase reset
