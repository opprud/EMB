PRG            = irq
OBJ            = boot.o VIClowlevel.o main.o
OPTIMIZE       = -g -o0
# USE_THUMB_MODE = YES

LDSCRIPT       = 2468_rom.ld
DEFS           =
LIBS           =


# You should not have to change anything below here.
# CROSS compile added to set correc t path on MAC
CROSS_COMPILE = /Users/MortenOJ/sat/bin/


# You should not have to change anything below here.
CC             = $(CROSS_COMPILE)/arm-none-eabi-gcc
OBJCOPY        = $(CROSS_COMPILE)/arm-none-eabi-objcopy
OBJDUMP        = $(CROSS_COMPILE)/arm-none-eabi-objdump
ARCH           = $(CROSS_COMPILE)/arm-none-eabi-ar


CFLAGS=-Wall -I. $(DEBUG) -mcpu=arm7tdmi -nostartfiles -T$(LDSCRIPT) \
		-Wcast-align $(OPTIMIZE) $(DEFS)

ifeq ($(USE_THUMB_MODE),YES)
	CFLAGS += -mthumb -mthumb-interwork
endif

ASFLAGS=-Wa

$(PRG).elf: $(OBJ) $(LDSCRIPT) makefile
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $(OBJ) -o $@

clean:
	rm -rf *.o *.lst $(PRG).elf $(PRG).hex $(PRG).bin

lst:  $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.o : %.s makefile
	$(CC) -c $(ASFLAGS) $< -o $@

%.o : %.c makefile
	$(CC) -c $(CFLAGS) $< -o $@

hex:  $(PRG).hex

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

bin: $(PRG).bin

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

run: $(PRG).hex
	jtag_flash $(PRG).hex

debug: $(PRG).hex
	jtag_flash $(PRG).hex N
	arm-elf-insight $(PRG).elf &
	jtag_stub
