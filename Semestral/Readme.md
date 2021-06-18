# Wolf and Stein (téma: Akční hra)
`Autor: Dominik Alfery`

## Téma z Progtestu

Implementujte jednoduchou střílecí hru alespoň jednoho hráče proti počítači.

Hra musí splňovat následující funkcionality:

Objekty, které je možné zničit (nepřátelské jednotky, statické barikády)
Pohybovat s hráčem (chůze panáčka, let vesmírné lodi, páčka na odrážení míčků)
Po zničení objektu s určitou pravděpodobností zůstane bonus, implementujte několik různých bonusů (munice, rychlejší nabíjení,...)
Implementujte jednoduchý "fyzikální" model (setrvačnost lodi, gravitace, odrážení, nabíjení zbrani,...)
Implementujte načítání (generátoru) mapy ze souboru.
Implementujte perzistenci (ukládání a načítání) tabulky s nejlepšími hráči do souboru.
Kde lze využít polymorfismus? (doporučené)

Rozdělení objektů: (ne)pohyblivý, hráč vs. nepřítel
Různé bonusy: munice, silnější zbraně, zdvojení zbraní, rychlejší přebíjení,...
Uživatelské rozhraní: konzole, ncurses, SDL, OpenGL (různé varianty), ...

## Popis hry

Hra Wolf and Stein bude zážitkově velmi podobná hře Wolfenstein. Plno protivníků a málo prostoru. Jen ve zjednodušeném grafickém prostředí. Což ale by skalním fanouškům v žádném případě vadit nemělo.
Při spuštění hry se uživateli zobrazí menu s výběrem možností.

- `New Game` - vytvoří novou hru
- `Scoreboard` - ukáže tabulku s výsledky seřazené od nejlepšího skóre 
- `Bonus`- popis a vysvětlení bonusů, které jde získat 
- `Quit` - ukončí hru

Pravidla hry budou velmi jednoduchá. Co nejdéle přežít a získat nejlepší skóre. Přičemž sbírat skóre za poražení různých protivníků.
Životy se budou ubírat po blízkém setkání s nepřítelem. Z mrtvých protivníků může vypadnout bonus - munice, životy či rychlejší nabíjení. 
Hráč uvidí svoje statistiky - počet munice v zásobníku/celkový počet munice, aktuální zdraví, získané skóre a aktuálně získaný bonus.
Při dosažení určitého skóre se změní level a zvýší úroveň nepřátel.

Příklad možného zobrazení hry - Hráč je reprezentován @, žíví nepřátelé jsou &, O a mrtví X. 

````
+--------------------+	  
|    	    @	     |	  
| 	    	     |
|    & 	    . 	     |	  Health: 80
|    	    .  	     |    Ammo: 7/35
|   	    X	     |    Score: 350	
|   X		     |    Bonus: None
|	O	  &  |    Level: 2
|	      	     |
|    	      O      |
|	     	     |
+--------------------+
````

Ovládání je zajištěno přes klávesnici ve třídě `CInterface`. Zde bude docházet i k výpisu do terminálu a celková práce s vstupem a výstupem. Tato třída je obsažena v `CApplication`, jenž je hlavní třídou starající se o spuštení aplikace. 
O chod hry se stará třída `CGame`, která spravuje postavy a spravuje hlavní loop hry. `CMap` třída reprezentuje danou mapu Levelu, která se načte z daného souboru ve složce přes `CMapBuilder` metodou Generate. O vytváření bonusů stará třída `CBonus`, která náhodně vrací bonus po zabití protivníka. 
Hráč a nepřátelské postavy jsou obsaženy ve třídě `CCharacter`. Hráč při výstřelu vytvoří kulku, kterou zastupuje třída `CBullet`. Pozice a koordinace zaobaluje struct `CCoord`, jenž ulehčuje práci při předávání pozic herních objektů. 
Tabulka TOP skóre je uložena v `CScoreboard`, kde se vždy porovná nové skóre a podle toho uloží v daném pořadí. Poslední 2 enum classy `EDirection` a `EField` se používají pro zlepšenou kontrolu a orientaci při čtení a psaní kódu.

## Kde implementuji polymorfismus?

Hlavní polymorfismus jsem implementoval v metodě Move pro třídu CCharacter. Oddělil jsem tak zde pohyb hráče od AI a předešel zbytečnému vytváření nové třídy.
Polymorfismus jsem trochu využil u třídy `CBonus`, kde je několik derivovaných třídy s různými bonusovými vlastnostmi. Při volání Use se vrátí vector s danými vylepšujícími vlastnostmi pro hráče.

Tato volání mi asi nejvíce usnadní zbytečné kopírování kódu a zjednodušší celkovou implementaci. Čímž se vyvaruji zbytečných chyb a následného dlouhého hledání.