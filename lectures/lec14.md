# Virtual Machine Monitors
Spara pekare till varje guest os:s interrupt descriptor table. När ett visst
OS körs så har man dess pekare lagrad. Så man tar det nuvarande operativsystemets
interrupt handler för den givna interrupten. En interrupt som måste gå genom 
hypervisor tar ungefär dubbelt så lång tid, men om man tänker att ett program bara 
gör systemanrop 10% av tiden så är det inte *så* illa.

Men hur blire med själva interrupt handlern för operativsystemet? 
Hur garanterar man att den inte sabbar hela hypervisorn? Man kan inte bara ploppa in 
en hypervisor space, man ska ju i teori kunna ha hur många lager av virtuella maskiner 
som helst. 

En sak man kan göra är att
låta OS OCH OS:ens applikaitoner vara i user space, och hypervisor vara i kernel 
space. Men det är inte så härligt att OS:en inte kan göra priviligerade saker, och 
är inte heller härligt att OS:en har samma privilegier som dess applikationer.

Hur virtualiseras minnet mellan OS:ar då? Jo, låt oss antag att en applikation för
en OS gör en page fault. Då går vi via hypervisor, hittar OS:ens page fault interrupt 
handler, och kör den. Då mappas en ny page till den, men den operationen är 
priviliged, så hypervisorn måste göra den mappningen. 

Vi går tillbaka till problemet att "både OS och dess applikationer är i user space". 
Vi låter istället applikationen tro att bara den är i user space. När vi sen går till OS:en
genom en priviligerad operation så skickas det till istället hypervisor, som ändrar page tables så att 
OS:en blir user space istället. Sen sker interrupt handling enligt det som beskrivits 
tidigare. Sen när interrupten har tagit slut så ändras tabellerna tillbaka så att 
OS verkar vara i kernel space igen.

> Aside: En hypervisor brukar säga "hej OS, du får högst 2 GB RAM, om du använder mer så
måste du hantera allt jävla skit själv". Detta är i kontrast till en OS som låter en 
process ha i princip hur mycket minne som helst. 

Finns hårdvaruoperationer som specifikt hypervisors kan ha nytta av i ovanstående
sammanhang, dvs när minnesvirtualisering.

Para-virtualisering: Du använder modifierade kernel-moduler istället för de ursprungliga,
för att förenkla virtualiseringen.

Finns typ 1 och typ 2 virtualisering. Kolla upp kanske

Nu kan vi utnyttja hårdvaran, filsystem o sånt kan separeras helt från varandra. Men 
hur kan man låta applikationer använda olika operativsystem? Jo, med hjälp av dockers/containers

Man kan också fejka vilken processor man har, men det kan kosta mycket mer. Men om 
vi kan emulera en skruttprocessor kan vi utnyttja en snabb processor på laptopen
istället, så utvecklingsmiljön kan snabbas upp (så man slipper arbeta på en 
skruttprocessor).
