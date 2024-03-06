* Bank cart header by Tursi
* based on original CRT0 by Insomniac for GCC ELF
* Designed for up to 24k of fixed space loaded to >A000

 def _start
 def _init_data
 
* cartridge header here - we dont expect a normal startup entry
* We can reserve 44 bytes for the header on every page - that must take us to the bank switch!
    aorg >6000
    
* because many bytes in the standard header are unused, and location is well known,
* we store our init data there. That way the CRT length can change without needing
* to change the elf2cart tool
    data 0xaa01,0x0100      * header, version, [program count, unused] - [] used as data start in RAM
    data 0x0000,_prog       * [power up list], program list - [] used as data address in ROM
    data 0x0000,0x0000      * [DSR list], [subprogram list] - [] used as data size, [] then is bss start
    data 0x0000,0x0000      * not standard header - bss length, fixed program start

* program list
_prog:
    data 0x0000             * next program
    data _start             * entry point
    byte 13                 * length of following name
    text 'TEE-EYE TAROT........'
    even

******* IMPORTANT *****
* 40 bytes used till this point (note title length!)
* Pad the above if necessary to get to 42 bytes so the cartridge headers work.

* Entry point for C runtime initilization code
*
* This will set initial values for data memory and otherwise create
* the conditions needed for the C environment
_start:
  clr @>6000          * force bank switch to the first page - must end at exactly 46 bytes!

* Disable interrupts
*
* This will disable console reset via keyboard, music playback,
* sprite automotion, etc. But will make the environment much simpler
* to work in. Users can re-enable interrupts later if they wish.
  limi 0

* Set initial workspace
*
* The registers will be located at the start of scratchpad memory 
  lwpi 0x8300

* Initialize data segment to known values
  li   r0,0x6002       * Point into the standard header as above
  mov  *r0+, r1        * R1 = Start of data section
  mov  *r0+, r2        * R2 = Location of initial data
  inct r0              * skip program list
  mov  *r0+, r3        * R3 = Size of data section
  jeq  data_copy_end   * If size is 0, skip the loop
  
  * Copy loop for data initialization
data_copy_top:
  mov  *r2+, *r1+      
  dect r3
  jgt  data_copy_top
data_copy_end:

* Initialize BSS segment to zeroes
  mov  *r0+, r1        * R1 = Start of BSS section
  mov  *r0, r2         * R2 = Size of BSS section
  jeq  bss_clear_end   * If size is 0, skip the loop

  * Clearing loop for BSS initialization
bss_clear_top:
  clr  *r1+
  dect r2
  jgt  bss_clear_top
bss_clear_end:

* now copy the rest of the fixed data into RAM
* we are lazy here and assume a load address of 0xA000
* we also assume the rest of this page and all of page 1-2
* for (almost) 24k. Anyway... to do this right we are going to
* need to work from scratchpad - copy into >8320

  li r0,>8320
  li r1,cpcode
cpsclp:
  mov *r1+,*r0+
  ci r1,cpend
  jne cpsclp

* destination in RAM
  li r0,>a000  

* start address in header block
  mov @>600E,R1

* do the copy - we will return
  bl @>8320

* Create the stack
  li sp, 0x4000
  
* prevent screen blank
  inc @0x83d6

* Start running C code
  bl @main

* reset the console on return
  blwp @0x0000

cpcode:
* start from R1, copy the rest of the page,
* then switch the page and copy the whole thing,
* then reset bank and return. Only copies first 24k
* (minus the header bits)
  li r2,>6002       * next page
  li r3,3           * number of iterations
cplp:
  mov *r1+,*r0+     * first copy loop
  ci r1,>8000
  jne cplp

  li r1,>6000       * restart copy
  clr *r2+          * but on next page, and increment
  dec r3            * check if more
  jne cplp

  clr @>6000        * restore page 0
  b *r11            * back to caller
cpend:
