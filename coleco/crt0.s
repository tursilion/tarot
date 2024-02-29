; crt0.s for Colecovision cart
; todo: need to implement spinnerInt stub or wrapper in the C code...

	.module crt0
	.globl _main
	.globl _vdpinit
	.globl _my_nmi
	.globl _vdpLimi
	.globl _spinnerInt

	.area _HEADER(ABS)
	.org 0x8000

	.db 0x55, 0xaa			; Title screen and 12 second delay - swap 0x55 and 0xaa not to skip it.
	.dw 0								; sprite table stuff? - rarely used by the BIOS as a pointer
	.dw 0								; unknown - rarely used as a pointer to a single byte of RAM by the BIOS.
	.dw 0								; unknown - frequently used in BIOS routines as a pointer to a memory location (data is both written to and read from there, at least 124 bytes are used - maybe this is where the bios routines store most of their data, though with the common value of 0 this would be in the BIOS ROM itself - strange).
	.dw 0								; unknown - rarely used as a pointer to a single byte of RAM by the BIOS.
	.dw start						; where to start execution of program.
	ei		; RST 0x08
	reti					
	ei		; RST 0x10
	reti
	ei		; RST 0x18
	reti
	ei		; RST 0x20
	reti
	ei		; RST 0x28
	reti
	ei		; RST 0x30
	reti

	ei		; RST 0x38 - spinner interrupt
	reti
;	jp spinner	; RST 0x38 - spinner interrupt (one or the other - padding is important)
;	nop
	
	jp nmi		; NMI
	.ascii " / / NOT"

start:
	; clear interrupt flag right off
	ld hl,#_vdpLimi
	ld (hl),#0
	; clear RAM before starting
	ld hl,#0x7000			; set copy source
	ld de,#0x7001			; set copy dest
	ld bc,#0x03ff			; set bytes to copy (1 less than size)
	ld (hl),#0				; set initial value (this gets copied through)
	ldir					; do it
	
	ld  sp, #0x7400			; Set stack pointer directly above top of memory.
	ld	bc,#0xFFFE			; switch in code bank
   	ld	a,(bc)				; note that this does NOT set the local pBank variable, user code still must do that!
	call gsinit				; Initialize global variables. (always at end of code bank, so need above bank switch)
	call _vdpinit			; Initialize something or other ;)
	call _main
	rst 0x0					; Restart when main() returns.

	.ascii "   ** By Tursi aka M.Brent HarmlessLion for CollectorVision/Imagic **"

	;; Ordering of segments for the linker - copied from sdcc crt0.s
	.area _HOME
	.area _CODE
	.area _INITIALIZER
	.area   _GSINIT
	.area   _GSFINAL
        
	.area _DATA
	.area _BSS
	.area _HEAP

	.area _BSS
_vdpLimi:		; 0x80 - interrupt set, other bits used by library
	.ds 1

    .area _CODE
nmi:
; all we do is set the MSB of _vdpLimi, and then check
; if the LSB is set. If so, we call user code now. if
; not, the library will deal with it when enabled.
	push af					; save flags (none affected, but play safe!)
	push hl
	
	ld hl,#_vdpLimi
	bit 0,(hl)				; check LSb (enable)
	jp z,notokay
	
; okay, full on call, save off the (other) regs
	push bc
	push de
	;push ix ; saved by callee
	push iy
	call _my_nmi			; call the lib version
	pop iy
	;pop ix
	pop de
	pop bc	
	jp clrup				

notokay:
	set 7,(hl)				; set MSb (flag)

clrup:
	pop hl					
	pop af
	retn
	
spinner:
; Spinner handler - call out to C code
	push af					; save flags
	push hl
	push bc
	push de
	;push ix ; saved by callee
	push iy
	
; to prevent the NMI from blowing us up, we'll run two paths
; this is still slightly racey if the user is enabling/disabling
; the NMI, so don't do that if you are using the spinner.
	ld hl,#_vdpLimi
	bit 0,(hl)				; small race here...?
	jp z,spin_nmi_blocked	; if it's blocked, just do the easy path

; NMI was enabled, so to reduce conflict, disable them now
	res 0,(hl)
	
; now call the C code - it should be safe
	call _spinnerInt		; NMI is already off, so just call the code version
	
; now before we re-enable NMIs, we need to check if they already happened
	ld hl,#_vdpLimi
	set 0,(hl)				; turn them back on first
	bit 7,(hl)				; did we miss one?
	jp z,spin_cleanup		; there was no NMI missed, then we are done
	
; else, just call the NMI handler before we go
	call _my_nmi
	jp spin_cleanup

; we come this way if the NMI wasn't enabled to begin with	
spin_nmi_blocked:	
	call _spinnerInt		; NMI is already off, so just call the code version
	
; and we come this way in both cases	
spin_cleanup:
	pop iy
	;pop ix
	pop de
	pop bc	
	pop hl					
	pop af
	ei
	reti

; soft function to be overridden if that's possible
_spinnerInt:
	ret

	.area _GSINIT
gsinit::
	.area _GSFINAL
	ret

;------------------------------------
; Some basic string functions, here cause
; it is already an asm file...
;------------------------------------

	.area _CODE

; void *memcpy(void *dest, const void *source, unsigned int count)
; stack has return, dest, src, count
; for LDIR, we need destination in DE, src in HL, and count in BC
_memcpy::
	push ix
    ld ix,#4
    add ix,sp	; ix points at dest

	ld e,(ix)
	ld d,1(ix)	; get dest
	ld l,2(ix)
	ld h,3(ix)	; get src
	ld c,4(ix)
	ld b,5(ix)	; get count

    ld a,c      ; check count for zero
    or a,b
    jr Z,$mcp1

	ldir

$mcp1:
	pop ix
	ret

;-------------------------------------------------

; void *memset(void *dest, unsigned int val, unsigned int count)
; stack has return, dest, val, count
_memset::
	push ix
    ld ix,#4
    add ix,sp	; stack points at dest

;:7: unsigned char *work = buf;
	ld l,(ix)
	ld h,1(ix)		; hl = pointer
	ld e,2(ix)
;	ld d,3(ix)		; e = ch
	ld c,4(ix)
	ld b,5(ix)		; bc = count

;:9: while (cnt&0x03) {
$0001:
	ld	a,c
	and	a, #0x03
	jr	Z,$0013

;:10: *(work++) = ch;
	ld	(hl),e
	inc	hl

;:11: --cnt;
	dec	bc
	jr	$0001

;:14: while (cnt) {
$0013:
$0004:
	ld	a,b
	or	a,c
	jr	Z,$0006

;:15: *(work++) = ch;
;:16: *(work++) = ch;
;:17: *(work++) = ch;
;:18: *(work++) = ch;
	ld	(hl),e
	inc	hl
	ld	(hl),e
	inc	hl
	ld	(hl),e
	inc	hl
	ld	(hl),e
	inc	hl

;:19: cnt -= 4;
	ld	a,c
	add	a,#0xFC
	ld	c,a
	ld	a,b
	adc	a,#0xFF
	ld	b,a
	jr	$0004

$0006:
;:22: return buf ;
;	ld	l,4 (ix)
;	ld	h,5 (ix)

	pop ix
	ret

;-------------------------------------------------

; void *memmove(void *dest, void *src, unsigned int count)
; stack has return, dest, src, count
; for LDIR/LDDR, we need destination in DE, src in HL, and count in BC
_memmove::
	push ix
    ld ix,#4
    add ix,sp	; stack points at dest

	ld e,(ix)
	ld d,1(ix)	; dest in de
	ld l,2(ix)
	ld h,3(ix)	; src in hl
	ld c,4(ix)
	ld b,5(ix)	; count in bc

    ld a,c      ; test bc for zero
    or a,b
    jp Z,00109$

;:46: if (((int)src < (int)dst) && ((((int)src) + cnt) > (int)dst))
	ld	a,l
	sub	a,e
	ld	a,h
	sbc	a,d
	jp	PO, 00139$  ; src-dest, jump if overflow (ie: src is greater)
	xor	a, #0x80
00139$:
	jp	P,00108$    ; change/check sign, jump if became positive... was negative (ie: not zero)

	push hl
    ld	a,c
	add	a,l
	ld	l,a
	ld	a,b
	adc	a,h
	ld	h,a         ; src=src+cnt

	ld	a,e
	sub	a,l
	ld	a,d
	sbc	a,h         ; src-dest, jump if no carry (ie: dest is greater)
	jr	NC,00110$

; decrementing, memmove case
;:49: d = ((char *)dst) + cnt - 1;
; need to add bc and decrement de
    ld a,c
    add a,e
    ld e,a
    ld a,b
    adc a,d
    ld d,a          ; dest = dest + cnt
	dec	de

;:50: s = ((char *)src) + cnt - 1;
; src (hl) just needs the -1 now...
	dec	hl

;:51: while (cnt--) {
;:52: *d-- = *s--;
    lddr
    pop hl          ; just to fix up the stack
	jr	00109$
	
; need to recover original hl before we do the following
00110$:
	pop hl

; Incrementing - memcpy case
; all registers needed are still intact
00108$:
;:56: d = dst;
;:57: s = src;

;:58: while (cnt--) {
;:59: *d++ = *s++;
    ldir

; exit
00109$:
;:63: return dst;
;	ld	l,4 (ix)
;	ld	h,5 (ix)
	
	pop ix
	ret

