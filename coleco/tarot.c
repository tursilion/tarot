/* Tarot Reader for The Flip Side BBS */
/* version 1.1 - ported to libti99 in 2024 */
// In retrospect it would have been faster to just compile the PC version...
// Also updated to remove the TI asm and work on ColecoVision
// there are no graphics... although these days there could be, ROMs are big enough

#include <vdp.h>
#include <kscan.h>
#include <string.h>

char cardn[40],cardm[160],cardmr[160];
char name[80];
char as[80];
int a,b,c,datapt;
int picked[11],cnum;

int main();
int getyn();
void getkey();
void docard();
int get12(int x);
void getsig();
void read(int x);
void readem();
void read1();
void readall();

// the actual card data.
const char CARDFS[] = {
    "The Magician*Skill, dimplomacy, "
    "self-confidence.*Disgrace, disquiet, inter"
    "nal conflict.*"
    "The High Priestess*Secrets, mystery, "
    "wisdom, science*Passion, ardour, conceit.*"
    "The Empress*Fruitfulness, action, initiati"
    "ve.*Light, truth"
    "understanding, public\nrejoicing.*"
    "The Emperor*Stability, power, protection, "
    "a\ngreat man.*"
    "Benevolence, compassion, credit,\nimmaturit"
    "y.*"
    "The Hierophant*Captivity, servitude, a fat"
    "her.*Society, "
    "understanding, over-kindness,\nweakness.*"
    "The Lovers*Attraction, love, beauty, trial"
    "s\novercome.*"
    "Failure, foolish plans.*"
    "The Chariot*Providence, triumph, vengence."
    "*Riot, dispute, "
    "litigation, defeat.*"
    "Strength*Power, energy, courage.*Abuse of "
    "power, weakness, "
    "dischord.*"
    "The Hermit*Prudence, treason, corruption.*"
    "Concealment, disguise, fear, paranoia.*"
    "Wheel of Fortune*Destiny, good luck, succe"
    "ss.*"
    "Abundance, bad luck, superfluity.*"
    "Justice*Equity, rightness, fairness.*Law, "
    "bigotry, excessive "
    "severity.*"
    "The Hanged Man*Wisdom, trials, divination."
    "*Selfishness, "
    "peer pressure.*"
    "Death*Change, new events replace old, end "
    "of\nold ways.*"
    "Inertia, stagnation, sleep.*"
    "Temperance*Economy, moderation, frugality,"
    "\naccomodation.*"
    "Religion, disunion, competing interests*"
    "The Devil*Violence, extraordinary efforts,"
    " force,\nreluctance "
    "to change.*Evilness, weakness, pettiness.*"
    "The Tower*Misery, distress, violent change"
    ",\ndisgrace.*"
    "Oppression, imprisonment, tyranny.*"
    "The Star*Hope, bright prospects, knowledge"
    ".*Arrogance, "
    "haughtiness, impotence.*"
    "The Moon*Hidden enemies, danger, darkness,"
    "\ndeception.*"
    "Instability, inconsistency, some\ndeception"
    ".*"
    "The Sun*Material happiness, marriage,\ncont"
    "entment.*"
    "Happiness, contentment.*"
    "Judgement*Change of position, renewal,\nrew"
    "ard/punishment.*"
    "Weakness, simplicity, deliberation.*"
    "The Fool*Folly, mania, intoxication,\nextra"
    "vagance, an "
    "important decision.*Negligence, absence, c"
    "arelessness,\nvanity.*"
    "The World*Success, a trip, a move.*Inertia"
    ", stagnation, "
    "permanence.*"
    "King of Wands*A dark, friendly man, honest"
    " and\nconscientious.*"
    "Good, yet severe, austere and tolerant.*"
    "Queen of Wands*A dark, friendly woman, cha"
    "ste, loving,\nhonourab"
    "le.*Jealousy, deceit, infidelity.*"
    "Knight of Wands*A friendly young man, depa"
    "rture,\nabsence, "
    "moving.*Division, interruption, dischord.*"
    "Page of Wands*A faithful young person, a l"
    "over, an\nenvoy, news.*"
    "Announcements, bad news, indecision and\nin"
    "stability.*"
    "Ten of Wands*Opression, disguise, bad tidi"
    "ngs, loss\nof a lawsuit.*"
    "Contrarities, difficulties, intrigues.*"
    "Nine of Wands*Strong defense, delay, expec"
    "tation of\na battle.*"
    "Obstacles, adversity, calamity.*"
    "Eight of Wands*Activity, swiftness, hope, "
    "love.*"
    "Jealousy, disputes, bad conscience,\nquarre"
    "ls.*"
    "Seven of Wands*Valour, dicussion, negotiat"
    "ions,\nsuccess.*"
    "Perplexity, embarrassment, anxiety.*"
    "Six of Wands*Victory, great news.*Apprehen"
    "sion, treachery, "
    "disloyalty,\nfear of loss.*"
    "Five of Wands*Imitation, the fight for mat"
    "erial\nwealth.*"
    "Litigation, disputes, trickery,\ncontradict"
    "ion.*"
    "Four of Wands*Harmony, prosperity, peace.*"
    "Prosperity, peace, beauty,\nembellishment.*"
    "Three of Wands*Established strength, comme"
    "rce, trade,\nexisting "
    "success.*End of troubles, suspension of\nad"
    "versity, hard work.*"
    "Two of Wands*Unhappiness despite material "
    "success\nand wealth.*"
    "Surprise, wonder, fear.*"
    "Ace of Wands*Creation, invention, a beginn"
    "ing.*"
    "A fall from power, ruin, clouded joy.*"
    "King of Cups*A fair, responsible man,\ncrea"
    "tive intelligence.*"
    "A dishonest, double-dealing man,\ninjustice"
    ", vice, scandal.*"
    "Queen of Cups*A good, honest, devoted woma"
    "n, wisdom,\nvirtue.*"
    "Untrustworthy or perverse woman,\ndishonour"
    ", depravity.*"
    "Knight of Cups*arrival of someone, maybe a"
    " messenger,\na "
    "proposition or invitation.*Trickery, subtl"
    "ety, fraud.*"
    "Page of Cups*A studious youth, news, medit"
    "ation,\nbusiness.*"
    "Attachment, deception.*"
    "Ten of Cups*Contentment, perfect repose, c"
    "lear\nconscience.*"
    "False heart, bad conscience, violence.*"
    "Nine of Cups*Contentment, physical well-be"
    "ing,\nvictory, "
    "satisfaction.*Loyalty, but including mista"
    "kes or\nimperfections.*"
    "Eight of Cups*Desertion of previous undert"
    "akings.*"
    "Great joy, happiness, feasting.*"
    "Seven of Cups*Division of concentration, t"
    "oo many\nprojects are "
    "being undertaken at once.*Desire, will, de"
    "termination, a "
    "project.*"
    "Six of Cups*Past happiness, memories.*The "
    "future, renewal.*"
    "Five of Cups*Loss, but something more rema"
    "ins. Too\nmuch "
    "concern over the loss while\nignoring what "
    "is left.*"
    "News, alliances, false projects.*"
    "Four of Cups*Weariness, disgust, dissatisf"
    "action\nwith all that "
    "is offered.*Novelty, new relations, new\nin"
    "structions.*"
    "Three of Cups*Good conclusion of a matter,"
    " victory,\nhealing.*"
    "Achievement, end.*"
    "Two of Cups*Love, friendship, sympathy.*Fa"
    "lse love, folly, "
    "a misunderstanding.*"
    "Ace of Cups*True joy, abundance, holiness."
    "*False feelings, "
    "instability, revolution*"
    "King of Swords*Judgement, power, authority"
    ", law.*"
    "Cruelty, evil intentions.*"
    "Queen of Swords*Widowhood, absense, steril"
    "ity,\nseparation.*"
    "Malice, bigotry, deceit.*"
    "Knight of Swords*Skill, bravery in defense"
    " or offense.*"
    "Incapacity, extravagance.*"
    "Page of Swords*Secrecy, spying, vigilance."
    "*Unprepared state, "
    "hostile spying.*"
    "Ten of Swords*Pain, sadness, desolation, r"
    "uthless\nenemy.*"
    "Temporary advantage, profit, or power.*"
    "Nine of Swords*Death, failure, deception, "
    "dispair.*"
    "Imprisonment, suspicion, shame.*"
    "Eight of Swords*Bad news, conflict, sickne"
    "ss.*Opposition, "
    "accident, treachery,\nfatality.*"
    "Seven of Swords*Lack of guard. Severe weak"
    "ening of\nbattle "
    "readiness.*Good advice, instruction.*"
    "Six of Swords*Journey by water, a route,\nc"
    "ommissionary.*"
    "Declaration, confession.*"
    "Five of Swords*Destruction, dishonour, los"
    "s.*Distruction, "
    "dishonour, loss.*"
    "Four of Swords*Vigilance, solitude, retrea"
    "t, exile.*"
    "Circumspection, economy, precaution.*"
    "Three of Swords*Removal, absense, delay, d"
    "ivision.*Error, "
    "loss, distraction, confusion.*"
    "Two of Swords*Conformity, friendship, bala"
    "nce of\npower.*"
    "Imposture, falsehood, disloyalty.*"
    "Ace of Swords*Triumph, conquest, great for"
    "ce.*Conquest, or "
    "great force, with negative\nresults.*"
    "King of Pentacles*Courage, lethargy, valou"
    "r.*Vice, weak"
    "ness, ugliness, corruption.*"
    "Queen of Pentacles*Generosity, magnificenc"
    "e, liberty.*"
    "Evil, suspicion, suspense, fear,\nmistrust."
    "*"
    "Knight of Pentacles*Utility, interest, res"
    "ponsibility.*"
    "Inertia, discouragement, carelessness.*"
    "Page of Pentacles*Application, study, refl"
    "ection.*"
    "Prodigality, liberality, luxury.*"
    "Ten of Pentacles*Gain, riches, family matt"
    "ers.*Chance, loss,"
    " robbery.*"
    "Nine of Pentacles*Prudence, safety, succes"
    "s, discernment.*"
    "Roguery, deception, bad faith.*"
    "Eight of Pentacles*Employment, craftsmansh"
    "ip, skill in\nbusiness.*"
    "Voided ambition, exaction.*"
    "Seven of Pentacles*Money, business, barter"
    ".*Anxiety about "
    "money.*"
    "Six of Pentacles*Gifts, gratification, att"
    "ention.*Desire, envy,"
    " jealousy, illusion.*"
    "Five of Pentacles*Material problems, desti"
    "tution.*Disorder, "
    "chaos, dischrord.*"
    "Four of Pentacles*Surety of possessions, h"
    "olding on to\nwhat "
    "one has, gifts, legacy or\ninheritance.*Sus"
    "pense, delay, "
    "opposition.*"
    "Three of Pentacles*Trade, skilled labour, "
    "reknown, glory.*"
    "Mediocrity, pettiness, weakness.*"
    "Two of Pentacles*Written messages, happine"
    "ss.*"
    "Forced happiness, written exchange.*"
    "Ace of Pentacles*Perfect contentment, ecst"
    "asy,\nintelligence, "
    "wealth.*Evil side of wealth, bad intellige"
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

// BBS replacements
#define rprint(p) printf("%s\n",p)
#define sndcr() putchar('\n')
#define rinput(x) gets(x, 80)

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
    rprint("Greetings!\n");
    sndcr();

    // secret test mode
    kscanfast(0);
    if (KSCAN_KEY == ' ') {
        readall();
    }

    rprint("I am Collee-Koe, the mystical sayer\nof sooth.");
    sndcr();
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
    getkey();
    return 0;
}

// return Y or N only
int getyn()
{
    int c;
    c=0;
    while((c!='0')&&(c!='1'))
    {
        kscanfast(0);
        c=KSCAN_KEY; if(c>90) c=c-32;
    }
    return(c);
}

// wait for a keypress
void getkey()
{
    rprint("---Press a key---");
    do {
        kscanfast(0);
    } while (KSCAN_KEY == 0xff);
}

// do the card reading
void docard()
{
    sndcr();
    rprint("Would you like a brief explanation?(1/0)");
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
        getkey();
        sndcr();
        rprint("A computer can only give general");
        rprint("meanings. Not all the meanings given");
        rprint("necessarily apply. You must decide for");
        rprint("yourself what the cards are trying");
        rprint("to tell you.");
        getkey();
    }
    random();
    getsig();
    sndcr();
    //      1234567890123456789012345678901234567890
    rprint("Move the joystick while shuffling,\nto better influence the cards.");
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
// the db is a list of strings terminated by *
// each card has three strings - name, meaning, reverse meaning
void read(int x)
{
    const unsigned char *p;
    
    p = CARDFS;
    for(int a=0; a<=x; a++) {
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
    sndcr();
    //      1234567890123456789012345678901234567890
    rprint("This card crosses you.. it is your\ncurrent main obstacle.");
    read1();
    //      1234567890123456789012345678901234567890
    rprint("This is beneath you.. it is your\nfoundation in the matter.");
    read1();
    //      1234567890123456789012345678901234567890
    rprint("This is above you.. it is a goal\nof sorts.");
    read1();
    //      1234567890123456789012345678901234567890
    rprint("This is behind you.. recent\npast events.");
    read1();
    //      1234567890123456789012345678901234567890
    rprint("This is before you.. soon to occur.");
    read1();
    //      1234567890123456789012345678901234567890
    rprint("This is your attitude/relationship\ntowards the matter.");
    read1();
    //      1234567890123456789012345678901234567890
    rprint("This is your home environment, people\nand events around you.");
    read1();
    //      1234567890123456789012345678901234567890
    rprint("This card represents your hopes and\nfears.");
    read1();
    //      1234567890123456789012345678901234567890
    rprint("This is the probable outcome, if\nnothing is changed.");
    read1();
    //      1234567890123456789012345678901234567890
    rprint("That is all. Thank you for visiting me..");
}

// present one card
void read1()
{
    int x,y,fl,a;
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
    read(x);
    sndcr();
    rprint(cardn);
    if(y) {
        rprint("This card is reversed.");
        rprint(cardmr);
    } else {
        rprint(cardm);
    }
    sndcr();
    getkey();
}

// as a test, dump all cards so formatting can be checked
void readall() {
    int x;
    for (x=0; x<78; ++x) {
        read(x);
        sndcr();
        rprint(cardn);
        rprint(cardm);
        rprint("Reversed:");
        rprint(cardmr);
        sndcr();
        getkey();
    }
}

