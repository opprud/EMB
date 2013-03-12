PRG            = blink
OBJ            = boot.o blink.o
OPTIMIZE       = -g
# USE_THUMB_MODE = YES

LDSCRIPT       = lpc2468-rom.ld
DEFS           =
LIBS           =


# You should not have to change anything below here.
CROSS_COMPILE = /Users/MortenOJ/sat/bin/

CC             = $(CROSS_COMPILE)/arm-none-eabi-gcc
OBJCOPY        = /Users/MortenOJ/sat/bin/arm-none-eabi-objcopy
OBJDUMP        = /Users/MortenOJ/sat/bin/arm-none-eabi-objdump
ARCH           = /Users/MortenOJ/sat/bin/arm-none-eabi-ar

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
