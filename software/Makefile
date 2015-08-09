###############################################################################
# Makefile for RoverMux
###############################################################################

## General Flags
PROJECT 		= rovermux
MCU 			= attiny13a
PMCU			= t13
PORT 			= usb
PROGRAMMER 		= dragon_isp
F_CPU 			= 9600000UL
TARGET 			= $(PROJECT).elf
AVRBIN 			= /opt/avr/bin
CC 			= $(AVRBIN)/avr-gcc
CPP 			= $(AVRBIN)/avr-g++
OBJCOPY			= $(AVRBIN)/avr-objcopy
OBJDUMP			= $(AVRBIN)/avr-objdump
SIZE			= $(AVRBIN)/avr-size
AVRDUDE			= avrdude

LFUSE			= 0x7a
HFUSE			= 0xff

## Objects that must be built in order to link
OBJECTS = main.o 

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=$(F_CPU) -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=$(PROJECT).map

## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss size

## Compile
main.o: main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	$(OBJCOPY) -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-$(OBJCOPY) $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	$(OBJDUMP) -h -S $< > $@

install: $(PROJECT).hex 
	$(AVRDUDE) -B 10 -p $(PMCU) -c $(PROGRAMMER) -P $(PORT) -v -U flash:w:$(PROJECT).hex -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m

## required for avr dragon to reset itself
delay: fuses
	@sleep 3

fuses:
	$(AVRDUDE) -p $(PMCU) -c $(PROGRAMMER) -P $(PORT) -v -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m

size: ${TARGET}
	@echo
	@$(SIZE) --mcu=$(MCU) $(TARGET)

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(PROJECT).elf dep/* $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss $(PROJECT).map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

