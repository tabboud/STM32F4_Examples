# STM32F4-Discovery Makefile for Mac OSX

SRCS=$(wildcard src/*.c) 
# Add assembly source files here or use $(wildcard *.s) for all .s files
S_SRCS =$(wildcard src/*.s)

# Project name
PROJ_NAME = blinkystm32lib
OUTPATH = build

OUTPATH := $(abspath $(OUTPATH))
BASEDIR := $(abspath ./)
STM32LIB_DIR := $(abspath ../stm32lib)
MKDIR_P = mkdir -p

# Add library code
SRCS+=$(wildcard $(STM32LIB_DIR)/STM32F4xx_HAL_Driver/Src/*.c) 
SRCS+=$(wildcard $(STM32LIB_DIR)/STM32F4xx_HAL_Driver/Src/Legacy/*.c) 

###################################################

# Check for valid float argument
# NOTE that you have to run make clan after
# changing these as hardfloat and softfloat are not
# binary compatible
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
#override FLOAT_TYPE = hard
override FLOAT_TYPE = soft
endif
endif

###################################################

AS=$(BINPATH)arm-none-eabi-as
CC=$(BINPATH)arm-none-eabi-gcc
LD=$(BINPATH)arm-none-eabi-gcc
OBJCOPY=$(BINPATH)arm-none-eabi-objcopy
OBJDUMP=$(BINPATH)arm-none-eabi-objdump
SIZE=$(BINPATH)arm-none-eabi-size

LINKER_SCRIPT = stm32f407vgt_flash.ld

CPU = -mcpu=cortex-m4 -mthumb

CFLAGS  = $(CPU) -c -std=gnu99 -g -O2 -Wall

# Libraries to link
LIBS = -lc -lm -lgcc -lnosys
LDFLAGS  = $(CPU) -mlittle-endian -mthumb-interwork -Wl,--gc-sections,-Map=$(OUTPATH)/$(PROJ_NAME).map,--cref --specs=nano.specs


ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
else
CFLAGS += -msoft-float
endif

# Default to STM32F407xx if no device is passed
ifeq ($(DEVICE_DEF), )
DEVICE_DEF = STM32F407xx
endif

# Add C_DEFS
CFLAGS += -D$(DEVICE_DEF) \
		  -DUSE_HAL_DRIVER

vpath %.a lib

# Includes
INCLUDE_PATHS = -I$(STM32LIB_DIR)/CMSIS/Include
INCLUDE_PATHS += -I$(STM32LIB_DIR)/CMSIS/STM32F4xx/Include
INCLUDE_PATHS += -I$(STM32LIB_DIR)/STM32F4xx_HAL_Driver/Inc
INCLUDE_PATHS += -I$(STM32LIB_DIR)/STM32F4xx_HAL_Driver/Inc/Legacy
INCLUDE_PATHS += -I$(BASEDIR)/inc

# Library paths
# LIBPATHS = -L$(BASEDIR)/lib/STM32F4xx_StdPeriph_Driver

OBJS = $(SRCS:.c=.o)
OBJS += $(S_SRCS:.s=.o)

###################################################

.PHONY: proj

all: dir proj
	$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf

proj: $(OUTPATH)/$(PROJ_NAME).elf

.s.o:
	$(AS) $(CPU) -o $(addprefix $(OUTPATH)/, $(notdir $@)) $<

.c.o:
	# $(info filename $(notdir $@))
	$(CC) $(CFLAGS) -std=gnu99 $(INCLUDE_PATHS) -o $(addprefix  $(OUTPATH)/, $(notdir $@)) $<

$(OUTPATH)/$(PROJ_NAME).elf: $(OBJS)
	$(LD) $(LDFLAGS) -T$(LINKER_SCRIPT) -o $@ $(addprefix $(OUTPATH)/, $(notdir $^)) $(LIBS) $(LD_SYS_LIBS)
	$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin
	$(OBJDUMP) -S --disassemble $(OUTPATH)/$(PROJ_NAME).elf > $(OUTPATH)/$(PROJ_NAME).dis

dir:
	$(MKDIR_P) $(OUTPATH)/src

clean:
	rm -f $(OUTPATH)/*.o
	rm -f $(OUTPATH)/src/*.o
	rm -f $(OUTPATH)/$(PROJ_NAME).elf
	rm -f $(OUTPATH)/$(PROJ_NAME).hex
	rm -f $(OUTPATH)/$(PROJ_NAME).bin
	rm -f $(OUTPATH)/$(PROJ_NAME).dis
	rm -f $(OUTPATH)/$(PROJ_NAME).map

flash:
	st-flash write build/$(PROJ_NAME).bin 0x08000000
	
