# Paths to TMS9900 compilation tools 
# ( Set in environment to override paths )
#TMS9900_DIR?=/home/tursilion/gcc9900/bin
TMS9900_DIR?=/home/tursilion/newtms9900-gcc/newgcc9900/bin
ELF2EA5_DIR?=/home/tursilion/gcc9900/bin
EA5_SPLIT_DIR?=/home/tursilion/gcc9900/bin
CLASSIC99_DSK1?=/mnt/d/classic99/DSK1/
LIBTI99?=/mnt/d/work/libti99ALL/buildti

# Full paths to the executables used
GAS=$(TMS9900_DIR)/tms9900-as
LD=$(TMS9900_DIR)/tms9900-ld
CC=$(TMS9900_DIR)/tms9900-gcc
CP=/usr/bin/cp
NAME=tarot

# Path to elf2cart conversion utility
ELF2CART=/mnt/d/work/ti/Elf2BankedCart/elf2bankedcart

# Flags used during linking
#
# Locate the code (.text section) and the data (.data section)
# TODO: a linker script is necessary to define the output sections (that will replace the list in the Coleco CRT0)
# That will also let us set the start addresses. The ELF should in theory contain section names which will let
# out custom cartridge builder figure out where things go.
# https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_node/ld_19.html
LDFLAGS_EA5=-M -T linker.ld

INCPATH=-I/mnt/d/work/libti99ALL
LIBPATH=-L/mnt/d/work/libti99ALL/buildti
LIBS=-lti99

# Note: no-function-cse prevents gcc from preferring registers for bl calls
C_FLAGS=-Os -std=c99 -c -s --save-temp -DTI99 -fno-builtin -fno-function-cse

# List of all files needed in executable
PREREQUISITES=$(OBJECT_LIST) $(objs)
  
.PHONY: all clean

# all the files
objs = crt0_ea5.o tarot.o

all: $(PREREQUISITES)
	$(LD) $(PREREQUISITES) $(LIBS) $(LIBPATH) $(LDFLAGS_EA5) -o $(NAME).ea5.elf > ea5.map
	$(ELF2CART) $(NAME).ea5.elf $(NAME)8.bin

# Recipe to clean all compiled objects
.phony clean:
	-rm *.o
	-rm *.elf
	-rm *.map
	-rm *.lst
	-rm *.sym
	-rm *.ihx
	-rm *.rom
	-rm *.bin
	-rm *.s
	-rm *.i
	
# Recipe to compile all assembly files

%.o: %.asm
	$(GAS) $< -o $@

# Recipe to compile all C files
%.o: %.c
	$(CC) $(CFLAGS) $(INCPATH) $< $(C_FLAGS) $(INCPATH) -o $@
	 
