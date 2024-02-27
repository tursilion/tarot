/* 
 64k banked cartridge with 32k RAM support
 Fixed bank is copied into 32k by the CRT0
 In RAM we have a maximum size of 24k.
 All other pages here become 8k.
*/

OUTPUT_FORMAT("elf32-tms9900", "elf32-tms9900",
              "elf32-tms9900")
OUTPUT_ARCH(tms9900)
ENTRY(_start)
SEARCH_DIR("/home/tursi/gcc9900/tms9900/lib");

/* banked offset of 46 bytes to account for header space */

MEMORY
{
    INIT(xr) : ORIGIN = 0x6000, LENGTH = 256

    FIXED(xrw) : ORIGIN = 0xA000, LENGTH = 24K
    STACK(xrw) : ORIGIN = 0x2000, LENGTH = 8K

    SCRATCH(rw) : ORIGIN = 0x8320, LENGTH = 224
    CARTCODE1(XR) : ORIGIN = 0x602e, LENGTH = 8146
    CARTCODE2(XR) : ORIGIN = 0x602e, LENGTH = 8146
    CARTCODE3(XR) : ORIGIN = 0x602e, LENGTH = 8146
    CARTGFX1(XR) : ORIGIN = 0x602e, LENGTH = 8146
    CARTGFX2(XR) : ORIGIN = 0x602e, LENGTH = 8146
    CARTXTRA1(XR) : ORIGIN = 0x602e, LENGTH = 8146
    CARTXTRA2(XR) : ORIGIN = 0x602e, LENGTH = 8146
    CARTSPACE(XR) : ORIGIN = 0x602e, LENGTH = 8146

    FLASH(XR) : ORIGIN = 0, LENGTH = 256K
}

SECTIONS {
    PROVIDE (__executable_start = 0xA000); 

    PROVIDE (stext = .);
    
    . = 0x6000;
    
    loader :
    {
        crt0_ea5.o
    } > INIT AT> FLASH

    . = 0xA000;
    fixed :
    {
        crt0_ea5.o
        tarot.o(.text .rodata)
        *libti99.a:*(.text .rodata)
    } > FIXED AT> FLASH
    
    PROVIDE (etext = .);
    
    bank1 :
    {
        PROVIDE(ebank1a = .);
    } > CARTGFX1 AT> FLASH
    
    . = 0x2080;
    PROVIDE(sdata = .);
    .data : { *(.data) } > STACK AT> FLASH
    PROVIDE(edata = .);
    
    PROVIDE(sbss = .);
    .bss :  { *(.bss) libti99.a:*(.bss)  } > STACK AT> FLASH

    PROVIDE(ebss = .);

    _end = .; PROVIDE (end = .);
}
