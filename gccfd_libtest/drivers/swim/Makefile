# ----------------------------------------------------------------------------

# 	Makefile for compiling the Getting Started project

#-------------------------------------------------------------------------------
#		User-modifiable options
#-------------------------------------------------------------------------------

# Chip & board used for compilation
# (can be overriden by adding CHIP=chip and BOARD=board to the command-line)
CHIP  = lpc2478
CPU   = arm7tdmi
BOARD = ea_oem
TARGET = libswim.a

# Trace level used for compilation
# (can be overriden by adding TRACE_LEVEL=#number to the command-line)
# TRACE_LEVEL_DEBUG      5
# TRACE_LEVEL_INFO       4
# TRACE_LEVEL_WARNING    3
# TRACE_LEVEL_ERROR      2
# TRACE_LEVEL_FATAL      1
# TRACE_LEVEL_NO_TRACE   0
TRACE_LEVEL = 5

# Optimization level, put in comment for debugging
OPTIMIZATION = -O0

#platform source
PLATFORM = ../../platform

#drivers source
DRIVERS = drivers

# Output file basename
#OUTPUT = rtc-$(BOARD)-$(CHIP)
OUTPUT = libswim

# Compile for all memories available on the board (this sets $(MEMORIES))
include $(PLATFORM)/board.mak
# Output directories
BIN = .
OBJ = obj

#-------------------------------------------------------------------------------
#		Tools
#-------------------------------------------------------------------------------

# Tool suffix when cross-compiling
CROSS_COMPILE = arm-none-eabi-

# Compilation tools
RM = rm -f
AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
SIZE = $(CROSS_COMPILE)size
STRIP = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
RANLIB = $(CROSS_COMPILE)ranlib


# Flags
INCLUDES  = -I$(PLATFORM)
INCLUDES += -I$(DRIVERS)

CFLAGS  = -Wall -Wcast-align -Wcast-qual -Wimplicit -Wnested-externs -Wpointer-arith -Wswitch -Wreturn-type
CFLAGS += -g $(OPTIMIZATION) $(INCLUDES) -mcpu=$(CPU) -D$(CHIP) -DTRACE_LEVEL=$(TRACE_LEVEL)
ASFLAGS = -g $(OPTIMIZATION) $(INCLUDES) -mcpu=$(CPU) -D$(CHIP) -D__ASSEMBLY__ 
LDFLAGS = -g $(OPTIMIZATION) -mcpu=$(CPU) -nostartfiles -Wall -Wcast-align 

#-------------------------------------------------------------------------------
#		Files
#-------------------------------------------------------------------------------

# Directories where source files can be found
#UTILITY = $(LPC_LIB)/utility
#PERIPH = $(LPC_LIB)/peripherals
#BOARDS = $(LPC_LIB)/boards

# Libary dirs
#LIBS    = drivers/swim/libswim_arm.a


#VPATH += $(UTILITY)
VPATH += $(PLATFORM)
VPATH += $(DRIVERS)

# Objects built from C source files
#C_OBJECTS = main.o
#C_OBJECTS += framework.o uart.o rprintf.o irq.o VIClowlevel.o rtc.o

C_OBJECTS = lpc_colors.o \
          lpc_fonts.o \
          lpc_helvr10.o \
          lpc_rom8x16.o \
          lpc_rom8x8.o \
          lpc_swim_font.o \
          lpc_swim_image.o \
          lpc_swim.o \
          lpc_winfreesystem14x16.o \
          lpc_x5x7.o \
          lpc_x6x13.o          

# Objects built from Assembly source files
#ASM_OBJECTS = boot.o
ASM_OBJECTS =


# Append OBJ and BIN directories to output filename
OUTPUT := $(BIN)/$(OUTPUT)
OUTPUT := $(BIN)/$(OUTPUT)



#-------------------------------------------------------------------------------
#		Rules
#-------------------------------------------------------------------------------
all: $(BIN) $(OBJ) $(MEMORIES) $(LIBS)

$(BIN) $(OBJ):
	mkdir $@

define RULES
C_OBJECTS_$(1) = $(addprefix $(OBJ)/$(1)_, $(C_OBJECTS))
ASM_OBJECTS_$(1) = $(addprefix $(OBJ)/$(1)_, $(ASM_OBJECTS))

$(1): $$(ASM_OBJECTS_$(1)) $$(C_OBJECTS_$(1))
	$(CC) $(LDFLAGS) -T"$(PLATFORM)/2468_rom.ld" -o $(OUTPUT)-$$@.elf $$^
	$(OBJCOPY) -O binary $(OUTPUT)-$$@.elf $(OUTPUT)-$$@.bin
	$(OBJCOPY) -O ihex $(OUTPUT)-$$@.elf $(OUTPUT)-$$@.hex
	$(SIZE) $$^ $(OUTPUT)-$$@.elf

$$(C_OBJECTS_$(1)): $(OBJ)/$(1)_%.o: %.c Makefile $(OBJ) $(BIN)
	$(CC) $(CFLAGS) -D$(1) -c -o $$@ $$<

$$(ASM_OBJECTS_$(1)): $(OBJ)/$(1)_%.o: %.S Makefile $(OBJ) $(BIN)
	$(CC) $(ASFLAGS) -D$(1) -c -o $$@ $$<
	
#----------------------------------------------------------------------
# BUILD RULES FOR LIBRARY
#----------------------------------------------------------------------
$(TARGET): $(OBJ) $(LIBS)
	$(AR) cr $@ $(OBJ)
ifdef LIBS
	set -x ; \
	for lib in $(LIBS) ; do \
	  contents=`$(AR) t $$lib` ; \
	  $(AR) x $$lib ; \
	  $(AR) r $@ $$contents ; \
	  $(RM) $$contents ; \
	done
endif
	$(RANLIB) $@

endef

$(foreach MEMORY, $(MEMORIES), $(eval $(call RULES,$(MEMORY))))

clean:
	-rm -f $(OBJ)/*.o $(BIN)/*.bin $(BIN)/*.elf
