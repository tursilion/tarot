The files are slightly wider than normal to get a few extra pixels in the conversion.

The total size of RLE packed color and pixel data is 343,119 bytes - so a 512k ROM will work.

With a better compression we might manage 256k... but no big deal. 512k will work everywhere. It'll take a bit more with packing since the cards are not all the same size anyway.

The numbering comes from the original images. It's not a good match for my table (and it's not clear why fool is 0 but magician is 2... anyway.) My suits are in a different order INSIDE the suit.

The output order needed for my cards text table is:

MAJOR ARCANA
    01-22

WANDS
    36-23 in reverse order
    
CUPS
    50-37 in reverse order
    
SWORDS
    64-51 in reverse order
    
PENTACLES
    78-65 in reverse order

The code is roughly 16k so we MIGHT be able to keep it all in one page on the ColecoVision - we should strive to anyway. At worst we can move the text to a different page to solve that issue. The TI version copies to RAM so has 24k to play with anyway, the table and graphics code should take less than 8k.

TI paging: pretty consistently takes 3 files per 8k page. We have 78*2=156 files (color and pattern), so that's 52 pages of graphics data, or 416k. (It may be slightly more as I found a few that only had room for two files.)

Coleco Paging: pretty consistently takes 6 files, some accept 7. So that's 26 pages with the same total as the TI. Since some accept 7 we could end up slightly smaller, and the TI version needs per-file lookup anyway, so we might as well go for it.

It might be quickest to write a program that can generate the include file and lookup table. It can fit as many per page as it can (doesn't have to try too hard - if a card doesn't fit, just check all the rest of them).

The include file is just a list of #include "CARD00.rlec.c", #include "CARD00.rlep.c", etc, with the BIN2INC header define. That just gives us the names.

The main output will need to be lines for the crt0 pager or the linker.ld showing which page each card is in. We also need a table for card lookups in order:

CARD_ADDRESS, CARD_BANK

Then we just need to update the code to switch to bitmap mode, only print to the bottom 8 rows, fit to 32 columns instead of 40, and load the appropriate card before printing the text.
