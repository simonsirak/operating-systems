Kom ihåg från multi-level page tables: Page table i sig vill man ska få plats på en page

Fork kopierar en process. Men man behöver inte kopiera de pages som är readonly. Så om man klassar mkt som readonly, och sedan 
skriver man till ngn sådan sida, först då kopierar man sidan och sätter dem till write (om den inte var readonly på riktigt).

Man brukar ha page-storlekar som matchar med hur stora sektioner hårddisken delas upp i.

Notera att flera användare finns i linux, men det är för filsystemet. R/W menas snarare för "user vs kernel"

TLB:n kan hantera missar via hårdvaran om det är x86, men om det dessutom är en page som inte finns i minnet, då måste OS 
ta över och undersöka vidare.

Replacement policies för swapping kmr ofta på tenta :P 

Om man antar att 20 % av sidorna används 80 % av tiden och kör random replacement, så kmr hit rate bli ganska hög ändå 
(se simulering på slides). Större chans att ngn av de sidorna finns kvar efter random replacement.

Optimal replacement policy: Om du vet sekvensen av page accesses i framtiden kan du välja den page access som finns 
längst bort i framtiden och swappa ut den.

Att implementera LRU kan bli dyrt. Man kommer behöva göra minnesoperationer när man ändrar kön av accessade tabeller. Detta 
görs varje gång du gör en minnesoperation. Så det är ju dyrt som faen och förstör ju hela poängen med paging o swapping. Så 
istället approximerar man LRU.

FIFO suger dase ju mer minne/sidor du har

