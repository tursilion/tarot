/* Tarot Reader for The Flip Side BBS */
/* version 1.1 - ported to libti99 in 2024 */
// In retrospect it would have been faster to just compile the PC version...
// Also updated to remove the TI asm and work on ColecoVision

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
 
int main(void);
unsigned char getyn(void);
void getkey(unsigned char suppress);
void docard(void);
unsigned char get12(unsigned char x);
void getsig(void);
void read(int x);
void readem(void);
void read1(const char *p);
//void readall(void);

// the actual card data.
const char CARDFS[] = {
    "The Fool*Folly, mania, intoxication,\nextra"
    "vagance, an "
    "important\ndecision.*Negligence, absence,\nc"
    "arelessness, vanity.*"
    "The Magician*Skill, dimplomacy,\n"
    "self-confidence.*Disgrace, disquiet, inter"
    "nal\nconflict.*"
    "The High Priestess*Secrets, mystery, "
    "wisdom,\nscience*Passion, ardour, conceit.*"
    "The Empress*Fruitfulness, action,\ninitiati"
    "ve.*Light, truth"
    "understanding,\npublic rejoicing.*"
    "The Emperor*Stability, power, protection,\n"
    "a great man.*"
    "Benevolence,compassion, credit,\nimmaturit"
    "y.*"
    "The Hierophant*Captivity, servitude, a fat"
    "her.*Society, "
    "understanding,\nover-kindness, weakness.*"
    "The Lovers*Attraction, love, beauty,\ntrial"
    "s overcome.*"
    "Failure, foolish plans.*"
    "The Chariot*Providence, triumph, vengence."
    "*Riot, dispute, "
    "litigation,\ndefeat.*"
    "Strength*Power, energy, courage.*Abuse of "
    "power, weakness,\n"
    "dischord.*"
    "The Hermit*Prudence, treason, corruption.*"
    "Concealment, disguise, fear,\nparanoia.*"
    "Wheel of Fortune*Destiny, good luck, succe"
    "ss.*"
    "Abundance, bad luck,\nsuperfluity.*"
    "Justice*Equity, rightness, fairness.*Law, "
    "bigotry, excessive\n"
    "severity.*"
    "The Hanged Man*Wisdom, trials, divination."
    "*Selfishness, "
    "peer pressure.*"
    "Death*Change, new events replace old,\nend "
    "of old ways.*"
    "Inertia, stagnation, sleep.*"
    "Temperance*Economy, moderation, frugality,"
    "\naccomodation.*"
    "Religion, disunion, competing\ninterests*"
    "The Devil*Violence, extraordinary efforts,"
    "force, reluctance "
    "to change.*Evilness, weakness, pettiness.*"
    "The Tower*Misery, distress, violent\nchange"
    ", disgrace.*"
    "Oppression, imprisonment,\ntyranny.*"
    "The Star*Hope, bright prospects,\nknowledge"
    ".*Arrogance, "
    "haughtiness,\nimpotence.*"
    "The Moon*Hidden enemies, danger,\ndarkness,"
    " deception.*"
    "Instability, inconsistency,\nsome deception"
    ".*"
    "The Sun*Material happiness, marriage,\ncont"
    "entment.*"
    "Happiness, contentment.*"
    "Judgement*Change of position, renewal,\nrew"
    "ard/punishment.*"
    "Weakness, simplicity,\ndeliberation.*"
    "The World*Success, a trip, a move.*Inertia"
    ", stagnation, "
    "permanence.*"
    "King of Wands*A dark, friendly man, honest"
    "\nand conscientious.*"
    "Good, yet severe, austere and\ntolerant.*"
    "Queen of Wands*A dark, friendly woman, cha"
    "ste, loving, honourab"
    "le.*Jealousy, deceit, infidelity.*"
    "Knight of Wands*A friendly young man, depa"
    "rture,absence, "
    "moving.*Division,interruption, dischord.*"
    "Page of Wands*A faithful young person, a\nl"
    "over, an envoy, news.*"
    "Announcements, bad news,\nindecision and in"
    "stability.*"
    "Ten of Wands*Opression, disguise, bad\ntidi"
    "ngs, loss of a lawsuit.*"
    "Contrarities, difficulties,\nintrigues.*"
    "Nine of Wands*Strong defense, delay,\nexpec"
    "tation of a battle.*"
    "Obstacles, adversity, calamity.*"
    "Eight of Wands*Activity, swiftness, hope, "
    "love.*"
    "Jealousy, disputes, bad\nconscience, quarre"
    "ls.*"
    "Seven of Wands*Valour, dicussion, negotiat"
    "ions,success.*"
    "Perplexity, embarrassment,\nanxiety.*"
    "Six of Wands*Victory, great news.*Apprehen"
    "sion, treachery,\n"
    "disloyalty, fear of loss.*"
    "Five of Wands*Imitation, the fight for\nmat"
    "erial wealth.*"
    "Litigation, disputes, trickery,\ncontradict"
    "ion.*"
    "Four of Wands*Harmony, prosperity, peace.*"
    "Prosperity, peace, beauty,\nembellishment.*"
    "Three of Wands*Established strength, comme"
    "rce, trade, existing "
    "success.*End of troubles, suspension of\nad"
    "versity, hard work.*"
    "Two of Wands*Unhappiness despite material\n"
    "success and wealth.*"
    "Surprise, wonder, fear.*"
    "Ace of Wands*Creation, invention, a\nbeginn"
    "ing.*"
    "A fall from power, ruin, cloudedjoy.*"
    "King of Cups*A fair, responsible man,\ncrea"
    "tive intelligence.*"
    "A dishonest, double-dealing man,injustice"
    ", vice, scandal.*"
    "Queen of Cups*A good, honest, devoted woma"
    "n,\nwisdom, virtue.*"
    "Untrustworthy or perverse woman,dishonour"
    ", depravity.*"
    "Knight of Cups*arrival of someone, maybe a"
    "\nmessenger, a "
    "proposition or invitation.*Trickery, subtl"
    "ety, fraud.*"
    "Page of Cups*A studious youth, news,\nmedit"
    "ation, business.*"
    "Attachment, deception.*"
    "Ten of Cups*Contentment, perfect repose,\nc"
    "lear conscience.*"
    "False heart, bad conscience,\nviolence.*"
    "Nine of Cups*Contentment, physical\nwell-be"
    "ing, victory, "
    "satisfaction.*Loyalty, but including mista"
    "kes\nor imperfections.*"
    "Eight of Cups*Desertion of previous\nundert"
    "akings.*"
    "Great joy, happiness, feasting.*"
    "Seven of Cups*Division of concentration, t"
    "oo\nmany projects are "
    "being\nundertaken at once.*Desire, will, de"
    "termination, a\n"
    "project.*"
    "Six of Cups*Past happiness, memories.*The "
    "future, renewal.*"
    "Five of Cups*Loss,but something more rema"
    "ins.Too much "
    "concern over the loss\nwhile ignoring what "
    "is left.*"
    "News, alliances, false projects.*"
    "Four of Cups*Weariness, disgust,\ndissatisf"
    "action with all that"
    "is offered.*Novelty, new relations, new\nin"
    "structions.*"
    "Three of Cups*Good conclusion of a matter,"
    "\nvictory, healing.*"
    "Achievement, end.*"
    "Two of Cups*Love, friendship, sympathy.*Fa"
    "lse love, folly, "
    "a\nmisunderstanding.*"
    "Ace of Cups*True joy, abundance, holiness."
    "*False feelings, "
    "instability,\nrevolution*"
    "King of Swords*Judgement, power, authority"
    ",\nlaw.*"
    "Cruelty, evil intentions.*"
    "Queen of Swords*Widowhood, absense, steril"
    "ity,\nseparation.*"
    "Malice, bigotry, deceit.*"
    "Knight of Swords*Skill, bravery in defense"
    " or\noffense.*"
    "Incapacity, extravagance.*"
    "Page of Swords*Secrecy, spying, vigilance."
    "*Unprepared state, "
    "hostile\nspying.*"
    "Ten of Swords*Pain, sadness, desolation,\nr"
    "uthless enemy.*"
    "Temporary advantage, profit, or power.*"
    "Nine of Swords*Death, failure, deception, "
    "\ndespair.*"
    "Imprisonment, suspicion, shame.*"
    "Eight of Swords*Bad news, conflict, sickne"
    "ss.*Opposition, "
    "accident, treachery,fatality.*"
    "Seven of Swords*Lack of guard. Severe weak"
    "ening\nof battle "
    "readiness.*Good advice, instruction.*"
    "Six of Swords*Journey by water, a route,\nc"
    "ommissionary.*"
    "Declaration, confession.*"
    "Five of Swords*Destruction, dishonour, los"
    "s.*Distruction, "
    "dishonour, loss.*"
    "Four of Swords*Vigilance, solitude, retrea"
    "t,\nexile.*"
    "Circumspection, economy,\nprecaution.*"
    "Three of Swords*Removal, absense, delay,\nd"
    "ivision.*Error, "
    "loss, distraction,\nconfusion.*"
    "Two of Swords*Conformity, friendship, bala"
    "nce\nof power.*"
    "Imposture, falsehood,\ndisloyalty.*"
    "Ace of Swords*Triumph, conquest, great for"
    "ce.*Conquest, or "
    "great force, with\nnegative results.*"
    "King of Pentacles*Courage, lethargy, valou"
    "r.*Vice, weak"
    "ness, ugliness,\ncorruption.*"
    "Queen of Pentacles*Generosity, magnificenc"
    "e,\nliberty.*"
    "Evil, suspicion, suspense, fear,mistrust."
    "*"
    "Knight of Pentacles*Utility, interest,\nres"
    "ponsibility.*"
    "Inertia, discouragement,\ncarelessness.*"
    "Page of Pentacles*Application, study, refl"
    "ection.*"
    "Prodigality, liberality, luxury.*"
    "Ten of Pentacles*Gain, riches, family matt"
    "ers.*Chance, loss,"
    " robbery.*"
    "Nine of Pentacles*Prudence, safety, succes"
    "s,\ndiscernment.*"
    "Roguery, deception, bad faith.*"
    "Eight of Pentacles*Employment, craftsmansh"
    "ip,\nskill in business.*"
    "Voided ambition, exaction.*"
    "Seven of Pentacles*Money, business, barter"
    ".*Anxiety about "
    "money.*"
    "Six of Pentacles*Gifts, gratification, att"
    "ention.*Desire, envy,"
    "jealousy, illusion.*"
    "Five of Pentacles*Material problems, desti"
    "tution.*Disorder, "
    "chaos, dischrord.*"
    "Four of Pentacles*Surety of possessions, h"
    "olding\non to what "
    "one has, gifts,\nlegacy or inheritance.*Sus"
    "pense, delay, "
    "opposition.*"
    "Three of Pentacles*Trade, skilled labour, "
    "reknown, glory.*"
    "Mediocrity, pettiness, weakness.*"
    "Two of Pentacles*Written messages,\nhappine"
    "ss.*"
    "Forced happiness, written\nexchange.*"
    "Ace of Pentacles*Perfect contentment, ecst"
    "asy,\nintelligence, "
    "wealth.*Evil side of wealth, bad\nintellige"
    "nce.*"
    "******"
};

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
	unsigned int z;
	unsigned int cnt;

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
#define sndcr() putchar('\n')
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
//    sndcr();
    printf("Greetings!\n\n");

    // secret test mode
//    kscanfast(0);
 //   if (KSCAN_KEY == ' ') {
 //       readall();
 //   }

    rprint("I am Collee-Koe, the mystical\nsayer of sooth.\n");
    //sndcr();
    rprint("Would you like me to tell you what the");
    rprint("cards have in store for you?\n(press 1(yes) or 0(no))");
    c=getyn();
    if(c=='1') docard();
    else {
        rprint("I am insulted!! Leave, then, and never");
        rprint("again darken my door!");
    }

//    strcpy(as,"DSK5.DOR");
//    ea5ld();
    getkey(0);
    
    // before we return, there's a bug in the crt0 packer and only page 0 has the reboot
    // (though this probably doesn't affect the Coleco version)
    exit();
    
    return 0;
}

// return Y or N only
unsigned char getyn()
{
    unsigned char c;
    c=0;
    while((c!='0')&&(c!='1'))
    {
        kscanfast(0);
        c=KSCAN_KEY; if(c>90) c=c-32;
    }
    return(c);
}

// wait for a keypress
void getkey(unsigned char suppress)
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
    //sndcr();
    rprint("\nWould you like a brief explanation?(1/0)");
    c=getyn();
    sndcr();
    if(c=='1')
    {
        rprint("Tarot reading is an ancient art that");
        rprint("goes back hundreds, if not thousands,");
        rprint("of years. Tarot cards are brightly");
        rprint("coloured cards with divinitory meanings");
        rprint("hidden in the pictures. The deck is");
        rprint("similar to a regular playing deck,");
        rprint("but larger.\n");
        //sndcr();
        rprint("There are two parts, called 'Arcanas'.");
        rprint("The 'Major Arcana' contains the picture");
        rprint("cards. These cards are all independant");
        rprint("of the others, and tend to have a");
        rprint("more definite meaning attached.Examples");
        rprint("are The Star, or the well-known Death.");
        rprint("The 'Minor Arcana' has four suits");
        rprint("(Swords, Wands, Cups and Pentacles),");
        rprint("each with 14 cards (A-10, Page, Knight,");
        rprint("Queen and King).\n");
        //sndcr();
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
    //      12345678901234567890123456789012
    rprint("Move the joystick while\nshuffling, to better influence\nthe cards.");
    while ((KSCAN_JOYX==0)&&(KSCAN_JOYY==0)) { joystfast(KSCAN_MODE_LEFT); }
    strcpy(as,"help the user solve their question");
    //rinput(as);
    c=0;
    rprint("Shuffling...");
    while(as[c])
    {
        joystfast(KSCAN_MODE_LEFT);
        as[c]+=KSCAN_JOYY + KSCAN_JOYX;
        while(as[c]--) rnd(5);
        c++;
    }
    sndcr();
    readem();
}

// get a number from 1-x
unsigned char get12(unsigned char x)
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
    unsigned char a1,a2,a3;
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
    printf("Your significator is %s\n\n", cardn);
    //sndcr();
    cnum=1;
}

// read a single card from the database
// the db is a list of strings terminated by *
// each card has three strings - name, meaning, reverse meaning
void read(int x)
{
    const unsigned char *p;
    
    p = CARDFS;
    for(unsigned char a=0; a<=x; a++) {
        // copy one card index
        
        // name
        unsigned char *out = cardn;
        while (*p != '*') {
            *(out++) = *(p++);
        }
        *out = '\0';
        ++p;
        
        // meaning
        out = cardm;
        while (*p != '*') {
            *(out++) = *(p++);
        }
        *out = '\0';
        ++p;

        // reversed
        out = cardmr;
        while (*p != '*') {
            *(out++) = *(p++);
        }
        *out = '\0';
        ++p;
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

    //sndcr();
    //      12345678901234567890123456789012
    read1("\nThis card crosses you.. it is\nyour current main obstacle.");
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
    volatile unsigned char y;
    // display the card. cardTable is P, then C ** BACKWARDS FROM TI **
    y=*((volatile unsigned char*)(cardTable[x*2].bank));
    RLEUnpack(gPattern, cardTable[x*2].p, 4096);
    y=*((volatile unsigned char*)(cardTable[x*2+1].bank));
    RLEUnpack(gColor, cardTable[x*2+1].p, 4096);
}

void drawCardReverse() {
    // this actually flips the card, rather than RLEing it again
    // count through the top half
    for (unsigned char row=0; row<8; ++row) {
        for (unsigned char col=4; col<28; ++col) {
            unsigned char buf[8];
            unsigned char buf2[8];
            
            int vdpsrc=row*32*8 + col*8 + gPattern;
            int vdpdst=(15-row)*32*8 + col*8 + gPattern;
            vdpmemread(vdpsrc, buf, 8);
            vdpmemread(vdpdst, buf2, 8);
            // invert write
            VDP_SET_ADDRESS_WRITE(vdpdst);
            for (signed char c=7; c>=0; --c) {
                VDPWD = buf[c];
            }
            VDP_SET_ADDRESS_WRITE(vdpsrc);
            for (signed char c=7; c>=0; --c) {
               VDPWD = buf2[c];
            }
     
            vdpsrc=row*32*8 + col*8 + gColor;
            vdpdst=(15-row)*32*8 + col*8 + gColor;
            vdpmemread(vdpsrc, buf, 8);
            vdpmemread(vdpdst, buf2, 8);
            // invert write
            VDP_SET_ADDRESS_WRITE(vdpdst);
            //VDPWA=vdpdst&0xff;
            //VDPWA=(vdpdst>>8)|0x40;
            for (signed char c=7; c>=0; --c) {
                VDPWD = buf[c];
            }
            VDP_SET_ADDRESS_WRITE(vdpsrc);
            for (signed char c=7; c>=0; --c) {
                VDPWD = buf2[c];
            }
        }
    }
}

// present one card
void read1(const char *p)
{
    unsigned char x,y,fl,a;
    
    // hide the old card
    vdpmemset(gColor, 0, 4*1024);
    vdpmemset(gImage+16*32, ' ', 8*32);
    
    // print the header
    printf("\n%s\n", p);
    
    fl=1;
    x=0;y=0;
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

#if 0
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
#endif

