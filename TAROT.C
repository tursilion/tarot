/* Tarot Deck v2.0. Original was Tee-Eye. 11 Jan 94 by Mike Brent
   [Flipper] */

#include "stdio.h"

int z, y, card, sex, age, sig, rev, total, unit;
char dat[12500];   /* to hold the card data. 1d array to save memory */
char x[80];   /* miscellaneous */
int val[78][2];  /* card weight values */
char used[79];  /* which cards have been used? */
FILE *out;

main()
{ /* main program */
init();
intro();
x[0]='Y';
while (x[0]=='Y')
{ out=-1;
  while (out==-1)
  { puts("\nEnter a filename to log to, or press enter for no log:");
    gets(x);
    out=NULL;
    if (x[0])
    { out=fopen(x,"w");
      if (out==NULL)
      { puts("\nCannot write to that file!\n");
        out=-1;
      }
    }
  }
  puts("\n\n");
  get_user();
  do_read();
  explain();
  if (out)
    fclose(out);
  puts("\n\nAnother reading? (y/N)\n");
  gets(x);
  if (x[0]=='y') x[0]='Y';
}
printf("\nTarot Deck (c)1994 by Mike Brent\n\n");
printf("If you like this program, lemme know!!\n\n");
puts("\nBye!\n");
}

init()
{ /* read the data in */
unit=fopen("cards.dat","r");
if (unit==0) fail("CARDS.DAT not found.");
puts("\n\nI am Ibbim, the mystical seer. Please wait while I find\n");
puts("my cards...");
z=0;
x[0]='1';
while (x[0]!='!')
{ fgets(x,80,unit);
  strcpy(&dat[z],x);
  z=z+strlen(&dat[z])-1;
  dat[z++]=0;
}
for (y=0; y<78; y++)
 for (z=0; z<2; z++)
 { fgets(x,5,unit);
   val[y][z]=atoi(x);
 }
}

intro()
{ /* introduction, and display info if desired */
puts("\n\nThank you for waiting.\n");
puts("Would you like a brief description of the tarot? (y/N)\n\n");
gets(x);
if ((x[0]=='y')||(x[0]=='Y'))
{ z=0; y=0;
  while (x[0]!='*')
  { strcpy(x,&dat[z]);
    z=z+strlen(x)+1;
    if (x[0]!='*') 
    { puts(x);
      putc('\n',stdout);
      y++;
      if (y>22)
      { puts("\n>Press enter to continue...<");
        gets(x);
        y=0;
      }
    }
  }
}
}

fail(x) char x[];
{ /* exit with error message */
puts("\n\n");
puts(x);
puts("\n\n");
exit(1);
}

get_user()
{ /* ask some questions a choose a significator */
puts("\nIn order that I may choose a significator, please answer\n");
puts("the following questions:\n\n");
for (y=0; y<78; y++)
 used[y]='n';
used[78]='y';
puts("How old are you:");
gets(x);
age=atoi(x);
while ((x[0]!='M')&&(x[0]!='F'))
{ puts("\n\nAre you (M)ale or (F)emale:");
  gets(x);
  if (x[0]>'Z') x[0]=x[0]-32;
}
if (x[0]=='F') sex=1; else sex=0;
puts("\n\nWould you most consider yourself:\n");
puts("\n1) Always prepared");
puts("\n2) Thoughtful or religious");
puts("\n3) Emotional or hot-tempered");
puts("\n4) Skillful or diligent\n");
while ((x[0]<'1')||(x[0]>'4'))
{ puts("\n(1-4):");
  gets(x);
}
x[0]=x[0]-49;
card=22+x[0]*14;
if (age<=15) card=card+3; else card=card+sex;
puts2("\n\nYour significator is:\n");
showcard(card,0);
}

do_read()
{ /* ask the question and say the sooth */
puts("\nPlease enter your question, to better influence the cards.\n>");
gets(x);
if (out) fputs(x,out);
puts2("\n\nShuffling cards...");
for (z=0; z<strlen(x); z++)
 for (y=0; y<x[z]; y++)
  rand();
total=0;
puts2("\n\n");
do_card("Card #1: General Influences");
do_card("Card #2: Current Obstacles");
do_card("Card #3: Aim or Ideal");
do_card("Card #4: The Foundation of the Matter");
do_card("Card #5: Recent Past");
do_card("Card #6: Near Future");
do_card("Card #7: Your Views");
do_card("Card #8: Views of Others");
do_card("Card #9: Hopes and Fears");
do_card("Card #10: Probable Outcome");
}

showcard(q,r) int q,r;
{ /* display the name of card number q, and for r, nothing if 0, 
     normal (+) description if 1, and negative (-) description if 2 */
char qw[5];
z=0; y=-1;
used[q]='y';
while (y<q)
{ strcpy(x,&dat[z]);
  z=z+strlen(x)+1;
  if (x[0]=='*')
  { strcpy(x,&dat[z]);
    z=z+strlen(x)+1;
    y++;
  }
}
puts2("\n\n");
puts2(x);
puts2("\n\n");
if (r)
{ strcpy(qw,"+"); 
  if (r==2) 
  { puts2("This card is reversed...\n\n");
    strcpy(qw,"-");
  }
  while (strcmp(qw,x)!=0)
  { strcpy(x,&dat[z]);
    z=z+strlen(x)+1;
  }
  strcpy(x,"fda");
  while (strlen(x)>1)
  { strcpy(x,&dat[z]);
    z=z+strlen(x)+1;
    if (x[1]!=0)
    { puts2(x);
      puts2("\n");
    }
  }
  puts2("\n\n");
}
}

do_card(s) char s[];
{ /* do a single card */
puts2("\n---------------------------------------\n\n");
puts2(s);
puts2("\n\n");
card=78;
while (used[card]=='y')
  card=rand()%78;
rev=rand()%2+1;
showcard(card,rev);
total=total+val[card][rev-1];
puts2("Press return to continue...");
gets(x);
}

explain()
{ /* explain if it was a good reading or not... */
puts2("\n=======================================\n\n");
puts2("The overall tone of the reading is ");
if (total>=10)
  puts2("very good.\nYou can expect good results.\n\n");
if ((total<10)&&(total>=2))
  puts2("favourable.\nThere should be few, if any, problems.\n\n");
if ((total<2)&&(total>-2))
  puts2("neutral.\nIt is a time of stillness, caution or change.\n\n");
if ((total<=-2)&&(total>-10))
{ puts2("negative.\nYou should be very careful, and weigh your choices\n");
  puts2("very carefully.\n\n");
}
if (total<=-10)
{ puts2("very negative.\nYou should stop and carefully consider all your\n");
  puts2("options. The cards are warning that something is not right.\n\n");
}
}

void puts2(x) char *x;
{ /* print the string to the screen and log file if active */
  puts(x);
  if (out)
    fputs(x,out);
}
