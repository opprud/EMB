PRG            = irq
OBJ            = boot.o VIClowlevel.o irq.o extint.o main.o
OPTIMIZE       = -g
# USE_THUMB_MODE = YES

LDSCRIPT       = 2468_rom.ld
DEFS           =
LIBS           =


# You should not have to change anything below here.

CC             = arm-none-eabi-gcc
OBJCOPY        = arm-none-eabi-objcopy
OBJDUMP        = arm-none-eabi-objdump
ARCH           = arm-none-eabi-ar

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
