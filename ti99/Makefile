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
objs = crt0_ea5.o cardinc.o tarot.o CARD01.rlec.o CARD01.rlep.o  CARD02.rlec.o CARD02.rlep.o  CARD03.rlec.o CARD03.rlep.o  CARD04.rlec.o CARD04.rlep.o  CARD05.rlec.o CARD05.rlep.o  CARD06.rlec.o CARD06.rlep.o  CARD07.rlec.o CARD07.rlep.o  CARD08.rlec.o CARD08.rlep.o  CARD09.rlec.o CARD09.rlep.o  CARD10.rlec.o CARD10.rlep.o  CARD11.rlec.o CARD11.rlep.o  CARD12.rlec.o CARD12.rlep.o  CARD13.rlec.o CARD13.rlep.o  CARD14.rlec.o CARD14.rlep.o  CARD15.rlec.o CARD15.rlep.o  CARD16.rlec.o CARD16.rlep.o  CARD17.rlec.o CARD17.rlep.o  CARD18.rlec.o CARD18.rlep.o  CARD19.rlec.o CARD19.rlep.o  CARD20.rlec.o CARD20.rlep.o  CARD21.rlec.o CARD21.rlep.o  CARD22.rlec.o CARD22.rlep.o  CARD23.rlec.o CARD23.rlep.o  CARD24.rlec.o CARD24.rlep.o  CARD25.rlec.o CARD25.rlep.o  CARD26.rlec.o CARD26.rlep.o  CARD27.rlec.o CARD27.rlep.o  CARD28.rlec.o CARD28.rlep.o  CARD29.rlec.o CARD29.rlep.o  CARD30.rlec.o CARD30.rlep.o  CARD31.rlec.o CARD31.rlep.o  CARD32.rlec.o CARD32.rlep.o  CARD33.rlec.o CARD33.rlep.o  CARD34.rlec.o CARD34.rlep.o  CARD35.rlec.o CARD35.rlep.o  CARD36.rlec.o CARD36.rlep.o  CARD37.rlec.o CARD37.rlep.o  CARD38.rlec.o CARD38.rlep.o  CARD39.rlec.o CARD39.rlep.o  CARD40.rlec.o CARD40.rlep.o  CARD41.rlec.o CARD41.rlep.o  CARD42.rlec.o CARD42.rlep.o  CARD43.rlec.o CARD43.rlep.o  CARD44.rlec.o CARD44.rlep.o  CARD45.rlec.o CARD45.rlep.o  CARD46.rlec.o CARD46.rlep.o  CARD47.rlec.o CARD47.rlep.o  CARD48.rlec.o CARD48.rlep.o  CARD49.rlec.o CARD49.rlep.o  CARD50.rlec.o CARD50.rlep.o  CARD51.rlec.o CARD51.rlep.o  CARD52.rlec.o CARD52.rlep.o  CARD53.rlec.o CARD53.rlep.o  CARD54.rlec.o CARD54.rlep.o  CARD55.rlec.o CARD55.rlep.o  CARD56.rlec.o CARD56.rlep.o  CARD57.rlec.o CARD57.rlep.o  CARD58.rlec.o CARD58.rlep.o  CARD59.rlec.o CARD59.rlep.o  CARD60.rlec.o CARD60.rlep.o  CARD61.rlec.o CARD61.rlep.o  CARD62.rlec.o CARD62.rlep.o  CARD63.rlec.o CARD63.rlep.o  CARD64.rlec.o CARD64.rlep.o  CARD65.rlec.o CARD65.rlep.o  CARD66.rlec.o CARD66.rlep.o  CARD67.rlec.o CARD67.rlep.o  CARD68.rlec.o CARD68.rlep.o  CARD69.rlec.o CARD69.rlep.o  CARD70.rlec.o CARD70.rlep.o  CARD71.rlec.o CARD71.rlep.o  CARD72.rlec.o CARD72.rlep.o  CARD73.rlec.o CARD73.rlep.o  CARD74.rlec.o CARD74.rlep.o  CARD75.rlec.o CARD75.rlep.o  CARD76.rlec.o CARD76.rlep.o  CARD77.rlec.o CARD77.rlep.o  CARD78.rlec.o CARD78.rlep.o

#note: nothing copies the card .o files from ../cardpics, do it manually
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
	 
