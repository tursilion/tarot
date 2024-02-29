bank3 :
{
    CARD01.rlec.o(.text .rodata)
    CARD01.rlep.o(.text .rodata)
    CARD02.rlec.o(.text .rodata)
    PROVIDE(ebank3 = .);
} > CART3 AT> FLASH

bank4 :
{
    CARD02.rlep.o(.text .rodata)
    CARD03.rlec.o(.text .rodata)
    CARD03.rlep.o(.text .rodata)
    PROVIDE(ebank4 = .);
} > CART4 AT> FLASH

bank5 :
{
    CARD04.rlec.o(.text .rodata)
    CARD04.rlep.o(.text .rodata)
    CARD05.rlec.o(.text .rodata)
    PROVIDE(ebank5 = .);
} > CART5 AT> FLASH

bank6 :
{
    CARD05.rlep.o(.text .rodata)
    CARD06.rlec.o(.text .rodata)
    CARD06.rlep.o(.text .rodata)
    PROVIDE(ebank6 = .);
} > CART6 AT> FLASH

bank7 :
{
    CARD07.rlec.o(.text .rodata)
    CARD07.rlep.o(.text .rodata)
    CARD08.rlec.o(.text .rodata)
    PROVIDE(ebank7 = .);
} > CART7 AT> FLASH

bank8 :
{
    CARD08.rlep.o(.text .rodata)
    CARD09.rlec.o(.text .rodata)
    CARD09.rlep.o(.text .rodata)
    PROVIDE(ebank8 = .);
} > CART8 AT> FLASH

bank9 :
{
    CARD10.rlec.o(.text .rodata)
    CARD10.rlep.o(.text .rodata)
    PROVIDE(ebank9 = .);
} > CART9 AT> FLASH

bank10 :
{
    CARD11.rlec.o(.text .rodata)
    CARD11.rlep.o(.text .rodata)
    CARD12.rlec.o(.text .rodata)
    PROVIDE(ebank10 = .);
} > CART10 AT> FLASH

bank11 :
{
    CARD12.rlep.o(.text .rodata)
    CARD13.rlec.o(.text .rodata)
    CARD13.rlep.o(.text .rodata)
    PROVIDE(ebank11 = .);
} > CART11 AT> FLASH

bank12 :
{
    CARD14.rlec.o(.text .rodata)
    CARD14.rlep.o(.text .rodata)
    CARD15.rlec.o(.text .rodata)
    PROVIDE(ebank12 = .);
} > CART12 AT> FLASH

bank13 :
{
    CARD15.rlep.o(.text .rodata)
    CARD16.rlec.o(.text .rodata)
    CARD16.rlep.o(.text .rodata)
    PROVIDE(ebank13 = .);
} > CART13 AT> FLASH

bank14 :
{
    CARD17.rlec.o(.text .rodata)
    CARD17.rlep.o(.text .rodata)
    CARD18.rlec.o(.text .rodata)
    PROVIDE(ebank14 = .);
} > CART14 AT> FLASH

bank15 :
{
    CARD18.rlep.o(.text .rodata)
    CARD19.rlec.o(.text .rodata)
    CARD19.rlep.o(.text .rodata)
    PROVIDE(ebank15 = .);
} > CART15 AT> FLASH

bank16 :
{
    CARD20.rlec.o(.text .rodata)
    CARD20.rlep.o(.text .rodata)
    CARD21.rlec.o(.text .rodata)
    PROVIDE(ebank16 = .);
} > CART16 AT> FLASH

bank17 :
{
    CARD21.rlep.o(.text .rodata)
    CARD22.rlec.o(.text .rodata)
    PROVIDE(ebank17 = .);
} > CART17 AT> FLASH

bank18 :
{
    CARD22.rlep.o(.text .rodata)
    CARD36.rlec.o(.text .rodata)
    PROVIDE(ebank18 = .);
} > CART18 AT> FLASH

bank19 :
{
    CARD36.rlep.o(.text .rodata)
    CARD35.rlec.o(.text .rodata)
    CARD35.rlep.o(.text .rodata)
    PROVIDE(ebank19 = .);
} > CART19 AT> FLASH

bank20 :
{
    CARD34.rlec.o(.text .rodata)
    CARD34.rlep.o(.text .rodata)
    CARD33.rlec.o(.text .rodata)
    CARD33.rlep.o(.text .rodata)
    PROVIDE(ebank20 = .);
} > CART20 AT> FLASH

bank21 :
{
    CARD32.rlec.o(.text .rodata)
    CARD32.rlep.o(.text .rodata)
    CARD31.rlec.o(.text .rodata)
    CARD31.rlep.o(.text .rodata)
    PROVIDE(ebank21 = .);
} > CART21 AT> FLASH

bank22 :
{
    CARD30.rlec.o(.text .rodata)
    CARD30.rlep.o(.text .rodata)
    CARD29.rlec.o(.text .rodata)
    PROVIDE(ebank22 = .);
} > CART22 AT> FLASH

bank23 :
{
    CARD29.rlep.o(.text .rodata)
    CARD28.rlec.o(.text .rodata)
    CARD28.rlep.o(.text .rodata)
    PROVIDE(ebank23 = .);
} > CART23 AT> FLASH

bank24 :
{
    CARD27.rlec.o(.text .rodata)
    CARD27.rlep.o(.text .rodata)
    CARD26.rlec.o(.text .rodata)
    PROVIDE(ebank24 = .);
} > CART24 AT> FLASH

bank25 :
{
    CARD26.rlep.o(.text .rodata)
    CARD25.rlec.o(.text .rodata)
    CARD25.rlep.o(.text .rodata)
    CARD24.rlec.o(.text .rodata)
    PROVIDE(ebank25 = .);
} > CART25 AT> FLASH

bank26 :
{
    CARD24.rlep.o(.text .rodata)
    CARD23.rlec.o(.text .rodata)
    CARD23.rlep.o(.text .rodata)
    CARD50.rlec.o(.text .rodata)
    PROVIDE(ebank26 = .);
} > CART26 AT> FLASH

bank27 :
{
    CARD50.rlep.o(.text .rodata)
    CARD49.rlec.o(.text .rodata)
    CARD49.rlep.o(.text .rodata)
    PROVIDE(ebank27 = .);
} > CART27 AT> FLASH

bank28 :
{
    CARD48.rlec.o(.text .rodata)
    CARD48.rlep.o(.text .rodata)
    CARD47.rlec.o(.text .rodata)
    CARD47.rlep.o(.text .rodata)
    PROVIDE(ebank28 = .);
} > CART28 AT> FLASH

bank29 :
{
    CARD46.rlec.o(.text .rodata)
    CARD46.rlep.o(.text .rodata)
    CARD45.rlec.o(.text .rodata)
    PROVIDE(ebank29 = .);
} > CART29 AT> FLASH

bank30 :
{
    CARD45.rlep.o(.text .rodata)
    CARD44.rlec.o(.text .rodata)
    CARD44.rlep.o(.text .rodata)
    PROVIDE(ebank30 = .);
} > CART30 AT> FLASH

bank31 :
{
    CARD43.rlec.o(.text .rodata)
    CARD43.rlep.o(.text .rodata)
    CARD42.rlec.o(.text .rodata)
    PROVIDE(ebank31 = .);
} > CART31 AT> FLASH

bank32 :
{
    CARD42.rlep.o(.text .rodata)
    CARD41.rlec.o(.text .rodata)
    CARD41.rlep.o(.text .rodata)
    PROVIDE(ebank32 = .);
} > CART32 AT> FLASH

bank33 :
{
    CARD40.rlec.o(.text .rodata)
    CARD40.rlep.o(.text .rodata)
    CARD39.rlec.o(.text .rodata)
    PROVIDE(ebank33 = .);
} > CART33 AT> FLASH

bank34 :
{
    CARD39.rlep.o(.text .rodata)
    CARD38.rlec.o(.text .rodata)
    CARD38.rlep.o(.text .rodata)
    PROVIDE(ebank34 = .);
} > CART34 AT> FLASH

bank35 :
{
    CARD37.rlec.o(.text .rodata)
    CARD37.rlep.o(.text .rodata)
    CARD64.rlec.o(.text .rodata)
    PROVIDE(ebank35 = .);
} > CART35 AT> FLASH

bank36 :
{
    CARD64.rlep.o(.text .rodata)
    CARD63.rlec.o(.text .rodata)
    CARD63.rlep.o(.text .rodata)
    PROVIDE(ebank36 = .);
} > CART36 AT> FLASH

bank37 :
{
    CARD62.rlec.o(.text .rodata)
    CARD62.rlep.o(.text .rodata)
    CARD61.rlec.o(.text .rodata)
    PROVIDE(ebank37 = .);
} > CART37 AT> FLASH

bank38 :
{
    CARD61.rlep.o(.text .rodata)
    CARD60.rlec.o(.text .rodata)
    CARD60.rlep.o(.text .rodata)
    PROVIDE(ebank38 = .);
} > CART38 AT> FLASH

bank39 :
{
    CARD59.rlec.o(.text .rodata)
    CARD59.rlep.o(.text .rodata)
    CARD58.rlec.o(.text .rodata)
    PROVIDE(ebank39 = .);
} > CART39 AT> FLASH

bank40 :
{
    CARD58.rlep.o(.text .rodata)
    CARD57.rlec.o(.text .rodata)
    CARD57.rlep.o(.text .rodata)
    CARD56.rlec.o(.text .rodata)
    PROVIDE(ebank40 = .);
} > CART40 AT> FLASH

bank41 :
{
    CARD56.rlep.o(.text .rodata)
    CARD55.rlec.o(.text .rodata)
    CARD55.rlep.o(.text .rodata)
    PROVIDE(ebank41 = .);
} > CART41 AT> FLASH

bank42 :
{
    CARD54.rlec.o(.text .rodata)
    CARD54.rlep.o(.text .rodata)
    CARD53.rlec.o(.text .rodata)
    CARD53.rlep.o(.text .rodata)
    PROVIDE(ebank42 = .);
} > CART42 AT> FLASH

bank43 :
{
    CARD52.rlec.o(.text .rodata)
    CARD52.rlep.o(.text .rodata)
    CARD51.rlec.o(.text .rodata)
    CARD51.rlep.o(.text .rodata)
    PROVIDE(ebank43 = .);
} > CART43 AT> FLASH

bank44 :
{
    CARD78.rlec.o(.text .rodata)
    CARD78.rlep.o(.text .rodata)
    CARD77.rlec.o(.text .rodata)
    PROVIDE(ebank44 = .);
} > CART44 AT> FLASH

bank45 :
{
    CARD77.rlep.o(.text .rodata)
    CARD76.rlec.o(.text .rodata)
    CARD76.rlep.o(.text .rodata)
    CARD75.rlec.o(.text .rodata)
    PROVIDE(ebank45 = .);
} > CART45 AT> FLASH

bank46 :
{
    CARD75.rlep.o(.text .rodata)
    CARD74.rlec.o(.text .rodata)
    CARD74.rlep.o(.text .rodata)
    PROVIDE(ebank46 = .);
} > CART46 AT> FLASH

bank47 :
{
    CARD73.rlec.o(.text .rodata)
    CARD73.rlep.o(.text .rodata)
    CARD72.rlec.o(.text .rodata)
    CARD72.rlep.o(.text .rodata)
    PROVIDE(ebank47 = .);
} > CART47 AT> FLASH

bank48 :
{
    CARD71.rlec.o(.text .rodata)
    CARD71.rlep.o(.text .rodata)
    CARD70.rlec.o(.text .rodata)
    PROVIDE(ebank48 = .);
} > CART48 AT> FLASH

bank49 :
{
    CARD70.rlep.o(.text .rodata)
    CARD69.rlec.o(.text .rodata)
    CARD69.rlep.o(.text .rodata)
    PROVIDE(ebank49 = .);
} > CART49 AT> FLASH

bank50 :
{
    CARD68.rlec.o(.text .rodata)
    CARD68.rlep.o(.text .rodata)
    CARD67.rlec.o(.text .rodata)
    CARD67.rlep.o(.text .rodata)
    PROVIDE(ebank50 = .);
} > CART50 AT> FLASH

bank51 :
{
    CARD66.rlec.o(.text .rodata)
    CARD66.rlep.o(.text .rodata)
    CARD65.rlec.o(.text .rodata)
    CARD65.rlep.o(.text .rodata)
