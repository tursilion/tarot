/* Tarot Reader for The Flip Side BBS */
/* version 1.1 - ported to libti99 in 2024 */
// In retrospect it would have been faster to just compile the PC version...

char cardn[40],cardm[160],cardmr[160];
char name[80];
char as[80];
int a,b,c,datapt;
int picked[11],cnum;

#include <vdp.h>
#include <kscan.h>
#include <string.h>
#include <system.h>
#include "cardinc.h"

int main();
int getyn();
void getkey(int suppress);
void docard();
int get12(int x);
void getsig();
void read(int x);
void readem();
void read1(const char *p);
void readall();

// random number generator
unsigned int seed;
unsigned int rndnum()
{
    // these are all the masks
#if 0
    /* 00 */       0x00,  //             0
        /* 01 */       0x01,  //             1
        /* 02 */       0x03,  //             3
        /* 03 */       0x06,  //             7
        /* 04 */       0x0C,  //            15
        /* 05 */       0x14,  //            31
        /* 06 */       0x30,  //            63
        /* 07 */       0x60,  //           127
        /* 08 */       0xB8,  //           255
        /* 09 */     0x0110,  //           511
        /* 10 */     0x0240,  //         1,023
        /* 11 */     0x0500,  //         2,047
        /* 12 */     0x0CA0,  //         4,095
        /* 13 */     0x1B00,  //         8,191
        /* 14 */     0x3500,  //        16,383
        /* 15 */     0x6000,  //        32,767
        /* 16 */     0xB400,  //        65,535
        /* 17 */ 0x00012000,  //       131,071
        /* 18 */ 0x00020400,  //       262,143
        /* 19 */ 0x00072000,  //       524,287
        /* 20 */ 0x00090000,  //     1,048,575
        /* 21 */ 0x00140000,  //     2,097,151
        /* 22 */ 0x00300000,  //     4,194,303
        /* 23 */ 0x00400000,  //     8,388,607
        /* 24 */ 0x00D80000,  //    16,777,215
        /* 25 */ 0x01200000,  //    33,554,431
        /* 26 */ 0x03880000,  //    67,108,863
        /* 27 */ 0x07200000,  //   134,217,727
        /* 28 */ 0x09000000,  //   268,435,575
        /* 29 */ 0x14000000,  //   536,870,911
        /* 30 */ 0x32800000,  // 1,073,741,823
        /* 31 */ 0x48000000,  // 2,147,483,647
        /* 32 */ 0xA3000000   // 4,294,967,295
#endif

        if(seed&1) {
            seed >>= 1;
            seed ^= 0xb400;
        } else {
            seed >>= 1;
        }

    return seed;
}
// randomize the seed
void random() {
    seed = VDP_INT_COUNTER;
    rndnum();
}
// return a random number with a cap
unsigned int rnd(unsigned int max) {
    return rndnum() % max;
}

// graphics
void RLEUnpack(unsigned int p, const unsigned char *buf, unsigned int nMax) {
	unsigned char z;
	int cnt;

	cnt = nMax;
	VDP_SET_ADDRESS_WRITE(p);
	while (cnt > 0) {
		z=*buf;
		if (z&0x80) {
			// run of byte
			buf++;
			z&=0x7f;
			if (z>cnt) z=cnt;
			raw_vdpmemset(*buf, z);
			buf++;
		} else {
			// sequence of data
			buf++;
			if (z>cnt) z=cnt;
			raw_vdpmemcpy(buf, z);
			buf+=z; // this will be wrong if we hit the limit, but we won't need it again
		}
		cnt-=z;
	}
}

// BBS replacements
#define rprint(p) printf("%s\n",p)
#define sndcr() printf("\n")
#define rinput(x) gets(x, 80)

// scroll only the bottom 8 lines for bitmap split screen
void scrn_scroll_last8() {
	// hacky, slow, but functional scroll that takes minimal memory
	unsigned char x[4];		// 4 byte buffer to speed it up
	int nLine = nTextEnd-nTextRow+1;    // size of one line
	for (int adr=nLine*17+gImage; adr<nTextEnd+gImage; adr+=4) {
		vdpmemread(adr, x, 4);
		vdpmemcpy(adr-nLine, x, 4);
	}
	vdpmemset(nTextRow+gImage, ' ', nLine);	// clear the last line
}

// main code
int main()
{ 

//    unit=fopen("DSK5.ONLINE","r");
//    if(unit==0) {
//        strcpy(as,"DSK5.FLP");
//        ea5ld();
//    }
//    fgets(name,80,unit);
//    fclose(unit);

    set_text();
    VDP_SET_REGISTER(VDP_REG_COL, (COLOR_WHITE<<4)|COLOR_DKBLUE);
    charsetlc();
    vdpmemset(gImage, ' ', 40*24);

//    strcpy(as,"Greetings!");
//    strcat(as,name);
//    rprint(as);
    printf("Greetings!\n");
    sndcr();

    // secret test mode
    kscanfast(0);
    if (KSCAN_KEY == ' ') {
        readall();
    }

    rprint("I am Tee-Eye, the mystical sayer\nof sooth.");
    sndcr();
    rprint("Would you like me to tell you what the");
    rprint("cards have in store for you?\n(press Y or N)");
    c=getyn();
    if(c=='Y') docard();
    else {
        rprint("I am insulted!! Leave, then, and never");
        rprint("again darken my door!");
    }

//    strcpy(as,"DSK5.DOR");
//    ea5ld();
    getkey(0);
    
    // before we return, there's a bug in the crt0 packer and only page 0 has the reboot
    exit();
    
    return 0;
}

// return Y or N only
int getyn()
{
    int c;
    c=0;
    while((c!='Y')&&(c!='N'))
    {
        kscanfast(0);
        c=KSCAN_KEY; if(c>90) c=c-32;
    }
    return(c);
}

// wait for a keypress
void getkey(int suppress)
{
    if (!suppress) {
        rprint("---Press a key---");
    }
    do {
        kscanfast(0);
    } while (KSCAN_KEY == 0xff);
}

// do the card reading
void docard()
{
    sndcr();
    rprint("Would you like a brief explanation?(Y/N)");
    c=getyn();
    sndcr();
    if(c=='Y')
    {
        rprint("Tarot reading is an ancient art that");
        rprint("goes back hundreds, if not thousands,");
        rprint("of years. Tarot cards are brightly");
        rprint("coloured cards with divinitory meanings");
        rprint("hidden in the pictures. The deck is");
        rprint("similar to a regular playing deck,");
        rprint("but larger.");
        sndcr();
        rprint("There are two parts, called 'Arcanas'.");
        rprint("The 'Major Arcana' contains the picture");
        rprint("cards. These cards are all independant");
        rprint("of the others, and tend to have a");
        rprint("more definite meaning attached.Examples");
        rprint("are The Star, or the well-known Death.");
        rprint("The 'Minor Arcana' has four suits");
        rprint("(Swords, Wands, Cups and Pentacles),");
        rprint("each with 14 cards (A-10, Page, Knight,");
        rprint("Queen and King).");
        sndcr();
        rprint("The cards are laid out and interpreted");
        rprint("by where and how they lay. This program");
        rprint("will take care of interpretation.");
        getkey(0);
        sndcr();
        rprint("A computer can only give general");
        rprint("meanings. Not all the meanings given");
        rprint("necessarily apply. You must decide for");
        rprint("yourself what the cards are trying");
        rprint("to tell you.");
        getkey(0);
    }
    random();
    getsig();
    sndcr();
    rprint("Please enter your question,\nto better influence the cards.");
    rinput(as);
    c=0;
    rprint("Shuffling...");
    while(as[c])
    {
        while(as[c]--) rnd(5);
        c++;
    }
    sndcr();
    readem();
}

// get a number from 1-x
int get12(int x)
{
    int c;
    c=0;
    while((c<'1')||(c>x+48)) {
        kscanfast(0);
        c = KSCAN_KEY;
    }
    return(c-48);
}

// ask the user for their significator
void getsig()
{
    int a1,a2,a3;
    sndcr();
    rprint("First we must choose a significator,\na card to represent you.");
    sndcr();
    rprint("Please answer these questions as\nhonestly as possible.");
    sndcr();
    rprint("How old are you?");
    rprint("1: 16 or under");
    rprint("2: 17-25");
    rprint("3: over 25");
    a1=get12(3);
    sndcr();
    rprint("Are you:");
    rprint("1: male");
    rprint("2: female");
    a2=get12(2);
    sndcr();
    rprint("Are you:");
    rprint("1: neutral");
    rprint("2: easy-going");
    rprint("3: easily angered");
    rprint("4: a deep thinker");
    a3=get12(4);
    sndcr();
    picked[0]=(a3-1)*14;
    if(a1!=3) picked[0]=picked[0]+(4-a1);
    else picked[0]=picked[0]+(a2-1);
    picked[0]=picked[0]+22;
    read(picked[0]);
    sndcr();
//    crlf=0;
//    rprint("Your significator is ");
//    crlf=1;
//    rprint(cardn);
    printf("Your significator is %s\n", cardn);
    sndcr();
    cnum=1;
}

// read a single card from the database
// this was originally asm cause it was easier to do it that way in c99
void read(int x)
{
    int a;
    __asm__ volatile (
        "li r0,CARDFS\n\t"
        "mov r0,@datapt"
        : : : "cc" 
    );
    for(a=0; a<=x; a++) {
        // these labels don't meet gcc standards but not gonna worry about it...
        __asm__ volatile (
            "mov @datapt,r0\n"
            "  li r1,cardn\n"
            "lp1q%=\n"
            "  clr r2\n"
            "  movb *r0+,r2\n"
            "  ci r2,42*256\n"
            "  jeq ahha%=\n"
            "  movb r2,*r1+\n"
            "  jmp lp1q%=\n"
            "ahha%=\n"
            "  clr r2\n"
            "  movb r2,*r1+\n"
            "  li r1,cardm\n"
            "lp2q%=\n"
            "  clr r2\n"
            "  movb *r0+,r2\n"
            "  ci r2,42*256\n"
            "  jeq ahha2q%=\n"
            "  movb r2,*r1+\n"
            "  jmp lp2q%=\n"
            "ahha2q%=\n"
            "  clr r2\n"
            "  movb r2,*r1+\n"
            "  li r1,cardmr\n"
            "lp3q%=\n"
            "  clr r2\n"
            "  movb *r0+,r2\n"
            "  ci r2,42*256\n"
            "  jeq ahha3q%=\n"
            "  movb r2,*r1+\n"
            "  jmp lp3q%=\n"
            "ahha3q%=\n"
            "  clr r2\n"
            "  movb r2,*r1+\n"
            "  mov r0,@datapt\n"
            : : : "r1","r2","cc"
        );
    }
}

// do the full reading
void readem()
{
    set_bitmap(0);
    nTextFlags = TEXT_WIDTH_32; // remove special purpose flags and make it dumb
    VDP_SET_REGISTER(VDP_REG_COL, COLOR_DKBLUE);
    // blank out the color and pattern tables
    vdpmemset(gColor, 0, 6*1024);
    vdpmemset(gPattern, 0, 6*1024);
    // we need to force the character set into the third block    
    gPattern+=0x1000;
    charsetlc();
    gPattern-=0x1000;
    // set up bitmap on the top, text on the bottom
    vdpwriteinc(gImage, 0, 512);
    vdpmemset(gImage+512, ' ', 32*8);
    // and give the text area some color
    vdpmemset(gColor+4*1024, 0xf0, 2*1024);
    
    // and change the scroll function
    scrn_scroll = scrn_scroll_last8;

    sndcr();
    //      12345678901234567890123456789012
    read1("This card crosses you.. it is\nyour current main obstacle.");
    //      12345678901234567890123456789012
    read1("This is beneath you.. it is yourfoundation in the matter.");
    //      12345678901234567890123456789012
    read1("This is above you.. it is a goalof sorts.");
    //      12345678901234567890123456789012
    read1("This is behind you.. recent\npast events.");
    //      12345678901234567890123456789012
    read1("This is before you.. soon to\noccur.");
    //      12345678901234567890123456789012
    read1("This is your attitude/relation-\nship towards the matter.");
    //      12345678901234567890123456789012
    read1("This is your home environment,\npeople and events around you.");
    //      12345678901234567890123456789012
    read1("This card represents your hopes\nand fears.");
    //      12345678901234567890123456789012
    read1("This is the probable outcome, ifnothing is changed.");
    //      12345678901234567890123456789012

    // hide the old card
    vdpmemset(gColor, 0, 4*1024);
    vdpmemset(gImage+16*32, ' ', 8*32);
    rprint("\nThat is all.\nThank you for visiting me..");
}

void drawCard(int x) {
    // display the card. cardTable is C, then P
    *((volatile unsigned char*)(cardTable[x*2+1].bank))=0;
    RLEUnpack(gPattern, cardTable[x*2+1].p, 4096);
    *((volatile unsigned char*)(cardTable[x*2].bank))=0;
    RLEUnpack(gColor, cardTable[x*2].p, 4096);
}

void drawCardReverse() {
    // this actually flips the card, rather than RLEing it again
    int off=32; // skip the first 4 columns, they are empty
    // count through the top half
    for (int row=0; row<8; ++row) {
        for (int col=4; col<28; ++col) {
            unsigned char buf[8];
            unsigned char buf2[8];
            
            int vdpsrc=row*32*8 + col*8 + gPattern;
            int vdpdst=(15-row)*32*8 + col*8 + gPattern;
            vdpmemread(vdpsrc, buf, 8);
            vdpmemread(vdpdst, buf2, 8);
            // invert write
            VDP_SET_ADDRESS_WRITE(vdpdst);
            for (int c=7; c>=0; --c) {
                VDPWD = buf[c];
            }
            VDP_SET_ADDRESS_WRITE(vdpsrc);
            for (int c=7; c>=0; --c) {
               VDPWD = buf2[c];
            }
     
            vdpsrc=row*32*8 + col*8 + gColor;
            vdpdst=(15-row)*32*8 + col*8 + gColor;
            vdpmemread(vdpsrc, buf, 8);
            vdpmemread(vdpdst, buf2, 8);
            // invert write
            //VDP_SET_ADDRESS_WRITE(vdpdst);    // this one causes a compiler error?? impossible reload
            VDPWA=vdpdst&0xff;
            VDPWA=(vdpdst>>8)|0x40;
            for (int c=7; c>=0; --c) {
                VDPWD = buf[c];
            }
            VDP_SET_ADDRESS_WRITE(vdpsrc);
            for (int c=7; c>=0; --c) {
                VDPWD = buf2[c];
            }
        }
    }
}

// present one card
void read1(const char *p)
{
    int x,y,fl,a;
    
    // hide the old card
    vdpmemset(gColor, 0, 4*1024);
    vdpmemset(gImage+16*32, ' ', 8*32);
    
    // print the header
    printf("\n%s\n", p);
    
    fl=1;
    while(fl)
    {
        x=rnd(78); y=rnd(2);
        fl=0;
        for(a=0; a<cnum; a++) {
            if(picked[a]==x) fl=1;
        }
    }
    picked[cnum++]=x;

    drawCard(x);
    
    // get the text
    read(x);
    
    // using printf here for tighter control of vertical spacing
    printf("%s\n", cardn);
    if(y) {
        printf("This card is reversed.\n");
        drawCardReverse();
        printf("%s", cardmr);
    } else {
        printf("%s", cardm);
    }
    getkey(1);  // suppress --PRESS ANY KEY--
}

// as a test, dump all cards so formatting can be checked
void readall() {
    int x,y,fl,a;
    
    set_bitmap(0);
    nTextFlags = TEXT_WIDTH_32; // remove special purpose flags and make it dumb
    VDP_SET_REGISTER(VDP_REG_COL, COLOR_DKBLUE);
    // blank out the color and pattern tables
    vdpmemset(gColor, 0, 6*1024);
    vdpmemset(gPattern, 0, 6*1024);
    // we need to force the character set into the third block    
    gPattern+=0x1000;
    charsetlc();
    gPattern-=0x1000;
    // set up bitmap on the top, text on the bottom
    vdpwriteinc(gImage, 0, 512);
    vdpmemset(gImage+512, ' ', 32*8);
    // and give the text area some color
    vdpmemset(gColor+4*1024, 0xf0, 2*1024);
    
    // and change the scroll function
    scrn_scroll = scrn_scroll_last8;

    for (x=0; x<78; ++x) {
        // hide the old card
        vdpmemset(gColor, 0, 4*1024);
        vdpmemset(gImage+16*32, ' ', 8*32);
        
        read(x);

        printf("\nThis card is visible.\nThis is line two.\n");
        drawCard(x);
        printf("%s\n",cardn);
        printf("%s\n",cardm);
        getkey(1);
        
        printf("\n\n\n\n\n\n\n\n");
        drawCardReverse();
        printf("This card is visible.\nThis is line two.\n");
        printf("%s\n",cardn);
        printf("Reversed:\n");
        printf("%s", cardmr);
        getkey(1);
    }
}

// the actual card data. Again, it was just easier
// to do it this way in c99, or at least I thought
// it was at the time...
void dummyfunc() {
    __asm__ volatile(
        "\n"
    "CARDFS text 'The Fool*Folly, mania, intoxication,\nextra'\n\t"
    "text 'vagance, an '\n\t"
    "text 'important\ndecision.*Negligence, absence,\nc'\n\t"
    "text 'arelessness, vanity.*'\n\t"
    "text 'The Magician*Skill, dimplomacy,\n'\n\t"
    "text 'self-confidence.*Disgrace, disquiet, inter'\n\t"
    "text 'nal\nconflict.*'\n\t"
    "text 'The High Priestess*Secrets, mystery, '\n\t"
    "text 'wisdom,\nscience*Passion, ardour, conceit.*'\n\t"
    "text 'The Empress*Fruitfulness, action,\ninitiati'\n\t"
    "text 've.*Light, truth'\n\t"
    "text 'understanding,\npublic rejoicing.*'\n\t"
    "text 'The Emperor*Stability, power, protection,\n'\n\t"
    "text 'a great man.*'\n\t"
    "text 'Benevolence,compassion, credit,\nimmaturit'\n\t"
    "text 'y.*'\n\t"
    "text 'The Hierophant*Captivity, servitude, a fat'\n\t"
    "text 'her.*Society, '\n\t"
    "text 'understanding,\nover-kindness, weakness.*'\n\t"
    "text 'The Lovers*Attraction, love, beauty,\ntrial'\n\t"
    "text 's overcome.*'\n\t"
    "text 'Failure, foolish plans.*'\n\t"
    "text 'The Chariot*Providence, triumph, vengence.'\n\t"
    "text '*Riot, dispute, '\n\t"
    "text 'litigation,\ndefeat.*'\n\t"
    "text 'Strength*Power, energy, courage.*Abuse of '\n\t"
    "text 'power, weakness,\n'\n\t"
    "text 'dischord.*'\n\t"
    "text 'The Hermit*Prudence, treason, corruption.*'\n\t"
    "text 'Concealment, disguise, fear,\nparanoia.*'\n\t"
    "text 'Wheel of Fortune*Destiny, good luck, succe'\n\t"
    "text 'ss.*'\n\t"
    "text 'Abundance, bad luck,\nsuperfluity.*'\n\t"
    "text 'Justice*Equity, rightness, fairness.*Law, '\n\t"
    "text 'bigotry, excessive\n'\n\t"
    "text 'severity.*'\n\t"
    "text 'The Hanged Man*Wisdom, trials, divination.'\n\t"
    "text '*Selfishness, '\n\t"
    "text 'peer pressure.*'\n\t"
    "text 'Death*Change, new events replace old,\nend '\n\t"
    "text 'of old ways.*'\n\t"
    "text 'Inertia, stagnation, sleep.*'\n\t"
    "text 'Temperance*Economy, moderation, frugality,'\n\t"
    "text '\naccomodation.*'\n\t"
    "text 'Religion, disunion, competing\ninterests*'\n\t"
    "text 'The Devil*Violence, extraordinary efforts,'\n\t"
    "text 'force, reluctance '\n\t"
    "text 'to change.*Evilness, weakness, pettiness.*'\n\t"
    "text 'The Tower*Misery, distress, violent\nchange'\n\t"
    "text ', disgrace.*'\n\t"
    "text 'Oppression, imprisonment,\ntyranny.*'\n\t"
    "text 'The Star*Hope, bright prospects,\nknowledge'\n\t"
    "text '.*Arrogance, '\n\t"
    "text 'haughtiness,\nimpotence.*'\n\t"
    "text 'The Moon*Hidden enemies, danger,\ndarkness,'\n\t"
    "text ' deception.*'\n\t"
    "text 'Instability, inconsistency,\nsome deception'\n\t"
    "text '.*'\n\t"
    "text 'The Sun*Material happiness, marriage,\ncont'\n\t"
    "text 'entment.*'\n\t"
    "text 'Happiness, contentment.*'\n\t"
    "text 'Judgement*Change of position, renewal,\nrew'\n\t"
    "text 'ard/punishment.*'\n\t"
    "text 'Weakness, simplicity,\ndeliberation.*'\n\t"
    "text 'The World*Success, a trip, a move.*Inertia'\n\t"
    "text ', stagnation, '\n\t"
    "text 'permanence.*'\n\t"
    "text 'King of Wands*A dark, friendly man, honest'\n\t"
    "text '\nand conscientious.*'\n\t"
    "text 'Good, yet severe, austere and\ntolerant.*'\n\t"
    "text 'Queen of Wands*A dark, friendly woman, cha'\n\t"
    "text 'ste, loving, honourab'\n\t"
    "text 'le.*Jealousy, deceit, infidelity.*'\n\t"
    "text 'Knight of Wands*A friendly young man, depa'\n\t"
    "text 'rture,absence, '\n\t"
    "text 'moving.*Division,interruption, dischord.*'\n\t"
    "text 'Page of Wands*A faithful young person, a\nl'\n\t"
    "text 'over, an envoy, news.*'\n\t"
    "text 'Announcements, bad news,\nindecision and in'\n\t"
    "text 'stability.*'\n\t"
    "text 'Ten of Wands*Opression, disguise, bad\ntidi'\n\t"
    "text 'ngs, loss of a lawsuit.*'\n\t"
    "text 'Contrarities, difficulties,\nintrigues.*'\n\t"
    "text 'Nine of Wands*Strong defense, delay,\nexpec'\n\t"
    "text 'tation of a battle.*'\n\t"
    "text 'Obstacles, adversity, calamity.*'\n\t"
    "text 'Eight of Wands*Activity, swiftness, hope, '\n\t"
    "text 'love.*'\n\t"
    "text 'Jealousy, disputes, bad\nconscience, quarre'\n\t"
    "text 'ls.*'\n\t"
    "text 'Seven of Wands*Valour, dicussion, negotiat'\n\t"
    "text 'ions,success.*'\n\t"
    "text 'Perplexity, embarrassment,\nanxiety.*'\n\t"
    "text 'Six of Wands*Victory, great news.*Apprehen'\n\t"
    "text 'sion, treachery,\n'\n\t"
    "text 'disloyalty, fear of loss.*'\n\t"
    "text 'Five of Wands*Imitation, the fight for\nmat'\n\t"
    "text 'erial wealth.*'\n\t"
    "text 'Litigation, disputes, trickery,\ncontradict'\n\t"
    "text 'ion.*'\n\t"
    "text 'Four of Wands*Harmony, prosperity, peace.*'\n\t"
    "text 'Prosperity, peace, beauty,\nembellishment.*'\n\t"
    "text 'Three of Wands*Established strength, comme'\n\t"
    "text 'rce, trade, existing '\n\t"
    "text 'success.*End of troubles, suspension of\nad'\n\t"
    "text 'versity, hard work.*'\n\t"
    "text 'Two of Wands*Unhappiness despite material\n'\n\t"
    "text 'success and wealth.*'\n\t"
    "text 'Surprise, wonder, fear.*'\n\t"
    "text 'Ace of Wands*Creation, invention, a\nbeginn'\n\t"
    "text 'ing.*'\n\t"
    "text 'A fall from power, ruin, cloudedjoy.*'\n\t"
    "text 'King of Cups*A fair, responsible man,\ncrea'\n\t"
    "text 'tive intelligence.*'\n\t"
    "text 'A dishonest, double-dealing man,injustice'\n\t"
    "text ', vice, scandal.*'\n\t"
    "text 'Queen of Cups*A good, honest, devoted woma'\n\t"
    "text 'n,\nwisdom, virtue.*'\n\t"
    "text 'Untrustworthy or perverse woman,dishonour'\n\t"
    "text ', depravity.*'\n\t"
    "text 'Knight of Cups*arrival of someone, maybe a'\n\t"
    "text '\nmessenger, a '\n\t"
    "text 'proposition or invitation.*Trickery, subtl'\n\t"
    "text 'ety, fraud.*'\n\t"
    "text 'Page of Cups*A studious youth, news,\nmedit'\n\t"
    "text 'ation, business.*'\n\t"
    "text 'Attachment, deception.*'\n\t"
    "text 'Ten of Cups*Contentment, perfect repose,\nc'\n\t"
    "text 'lear conscience.*'\n\t"
    "text 'False heart, bad conscience,\nviolence.*'\n\t"
    "text 'Nine of Cups*Contentment, physical\nwell-be'\n\t"
    "text 'ing, victory, '\n\t"
    "text 'satisfaction.*Loyalty, but including mista'\n\t"
    "text 'kes\nor imperfections.*'\n\t"
    "text 'Eight of Cups*Desertion of previous\nundert'\n\t"
    "text 'akings.*'\n\t"
    "text 'Great joy, happiness, feasting.*'\n\t"
    "text 'Seven of Cups*Division of concentration, t'\n\t"
    "text 'oo\nmany projects are '\n\t"
    "text 'being\nundertaken at once.*Desire, will, de'\n\t"
    "text 'termination, a\n'\n\t"
    "text 'project.*'\n\t"
    "text 'Six of Cups*Past happiness, memories.*The '\n\t"
    "text 'future, renewal.*'\n\t"
    "text 'Five of Cups*Loss,but something more rema'\n\t"
    "text 'ins.Too much '\n\t"
    "text 'concern over the loss\nwhile ignoring what '\n\t"
    "text 'is left.*'\n\t"
    "text 'News, alliances, false projects.*'\n\t"
    "text 'Four of Cups*Weariness, disgust,\ndissatisf'\n\t"
    "text 'action with all that'\n\t"
    "text 'is offered.*Novelty, new relations, new\nin'\n\t"
    "text 'structions.*'\n\t"
    "text 'Three of Cups*Good conclusion of a matter,'\n\t"
    "text '\nvictory, healing.*'\n\t"
    "text 'Achievement, end.*'\n\t"
    "text 'Two of Cups*Love, friendship, sympathy.*Fa'\n\t"
    "text 'lse love, folly, '\n\t"
    "text 'a\nmisunderstanding.*'\n\t"
    "text 'Ace of Cups*True joy, abundance, holiness.'\n\t"
    "text '*False feelings, '\n\t"
    "text 'instability,\nrevolution*'\n\t"
    "text 'King of Swords*Judgement, power, authority'\n\t"
    "text ',\nlaw.*'\n\t"
    "text 'Cruelty, evil intentions.*'\n\t"
    "text 'Queen of Swords*Widowhood, absense, steril'\n\t"
    "text 'ity,\nseparation.*'\n\t"
    "text 'Malice, bigotry, deceit.*'\n\t"
    "text 'Knight of Swords*Skill, bravery in defense'\n\t"
    "text ' or\noffense.*'\n\t"
    "text 'Incapacity, extravagance.*'\n\t"
    "text 'Page of Swords*Secrecy, spying, vigilance.'\n\t"
    "text '*Unprepared state, '\n\t"
    "text 'hostile\nspying.*'\n\t"
    "text 'Ten of Swords*Pain, sadness, desolation,\nr'\n\t"
    "text 'uthless enemy.*'\n\t"
    "text 'Temporary advantage, profit, or power.*'\n\t"
    "text 'Nine of Swords*Death, failure, deception, '\n\t"
    "text '\ndespair.*'\n\t"
    "text 'Imprisonment, suspicion, shame.*'\n\t"
    "text 'Eight of Swords*Bad news, conflict, sickne'\n\t"
    "text 'ss.*Opposition, '\n\t"
    "text 'accident, treachery,fatality.*'\n\t"
    "text 'Seven of Swords*Lack of guard. Severe weak'\n\t"
    "text 'ening\nof battle '\n\t"
    "text 'readiness.*Good advice, instruction.*'\n\t"
    "text 'Six of Swords*Journey by water, a route,\nc'\n\t"
    "text 'ommissionary.*'\n\t"
    "text 'Declaration, confession.*'\n\t"
    "text 'Five of Swords*Destruction, dishonour, los'\n\t"
    "text 's.*Distruction, '\n\t"
    "text 'dishonour, loss.*'\n\t"
    "text 'Four of Swords*Vigilance, solitude, retrea'\n\t"
    "text 't,\nexile.*'\n\t"
    "text 'Circumspection, economy,\nprecaution.*'\n\t"
    "text 'Three of Swords*Removal, absense, delay,\nd'\n\t"
    "text 'ivision.*Error, '\n\t"
    "text 'loss, distraction,\nconfusion.*'\n\t"
    "text 'Two of Swords*Conformity, friendship, bala'\n\t"
    "text 'nce\nof power.*'\n\t"
    "text 'Imposture, falsehood,\ndisloyalty.*'\n\t"
    "text 'Ace of Swords*Triumph, conquest, great for'\n\t"
    "text 'ce.*Conquest, or '\n\t"
    "text 'great force, with\nnegative results.*'\n\t"
    "text 'King of Pentacles*Courage, lethargy, valou'\n\t"
    "text 'r.*Vice, weak'\n\t"
    "text 'ness, ugliness,\ncorruption.*'\n\t"
    "text 'Queen of Pentacles*Generosity, magnificenc'\n\t"
    "text 'e,\nliberty.*'\n\t"
    "text 'Evil, suspicion, suspense, fear,mistrust.'\n\t"
    "text '*'\n\t"
    "text 'Knight of Pentacles*Utility, interest,\nres'\n\t"
    "text 'ponsibility.*'\n\t"
    "text 'Inertia, discouragement,\ncarelessness.*'\n\t"
    "text 'Page of Pentacles*Application, study, refl'\n\t"
    "text 'ection.*'\n\t"
    "text 'Prodigality, liberality, luxury.*'\n\t"
    "text 'Ten of Pentacles*Gain, riches, family matt'\n\t"
    "text 'ers.*Chance, loss,'\n\t"
    "text ' robbery.*'\n\t"
    "text 'Nine of Pentacles*Prudence, safety, succes'\n\t"
    "text 's,\ndiscernment.*'\n\t"
    "text 'Roguery, deception, bad faith.*'\n\t"
    "text 'Eight of Pentacles*Employment, craftsmansh'\n\t"
    "text 'ip,\nskill in business.*'\n\t"
    "text 'Voided ambition, exaction.*'\n\t"
    "text 'Seven of Pentacles*Money, business, barter'\n\t"
    "text '.*Anxiety about '\n\t"
    "text 'money.*'\n\t"
    "text 'Six of Pentacles*Gifts, gratification, att'\n\t"
    "text 'ention.*Desire, envy,'\n\t"
    "text 'jealousy, illusion.*'\n\t"
    "text 'Five of Pentacles*Material problems, desti'\n\t"
    "text 'tution.*Disorder, '\n\t"
    "text 'chaos, dischrord.*'\n\t"
    "text 'Four of Pentacles*Surety of possessions, h'\n\t"
    "text 'olding\non to what '\n\t"
    "text 'one has, gifts,\nlegacy or inheritance.*Sus'\n\t"
    "text 'pense, delay, '\n\t"
    "text 'opposition.*'\n\t"
    "text 'Three of Pentacles*Trade, skilled labour, '\n\t"
    "text 'reknown, glory.*'\n\t"
    "text 'Mediocrity, pettiness, weakness.*'\n\t"
    "text 'Two of Pentacles*Written messages,\nhappine'\n\t"
    "text 'ss.*'\n\t"
    "text 'Forced happiness, written\nexchange.*'\n\t"
    "text 'Ace of Pentacles*Perfect contentment, ecst'\n\t"
    "text 'asy,\nintelligence, '\n\t"
    "text 'wealth.*Evil side of wealth, bad\nintellige'\n\t"
    "text 'nce.*'\n\t"
    "text '******'\n\t"
    );
}