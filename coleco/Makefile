# this just builds a simple, non-bank switched 32k ROM
# in order to prove the toolchain and demonstrate a basic project

CC = "D:/work/coleco/sdcc20230715/bin/sdcc"
CFLAGS = -mz80 -c --std-sdcc99 --opt-code-speed --no-builtin -Id:/work/libti99all -DCOLECO
AS = "D:/work/coleco/sdcc20230715/bin/sdasz80"
AFLAGS = -plosgff

# there is a build warning for duplicate symbols, but first one wins
LIB = -ld:/work/libti99all/buildcoleco/libti99_coleco.a

.PHONY: all clean

# crt0 must be first! (note this is also why the output is named crt0.ihx)
# list all your object files in this variable
objs = crt0.rel tarot.rel

# this rule calls makemegacart to do the final build of the ROM
all: buildcoleco
	makemegacart.exe crt0.ihx tarot.rom

# this rule links the files into the ihx
buildcoleco: $(objs)
	$(CC) -mz80 --no-std-crt0 --code-loc 0x8100 --data-loc 0x7000 $(objs) $(LIB)

# clean up the build
clean:
	-rm *.rel *.map *.lst *.sym *.asm *.ihx *.lk *.noi

# build the crt0 startup code
crt0.rel: crt0.s
	$(AS) $(AFLAGS) crt0.rel crt0.s

# build the source files - I can't remember why I broke them out like this... ;)
tarot.rel: tarot.c
	$(CC) $(CFLAGS) -o tarot.rel tarot.c
