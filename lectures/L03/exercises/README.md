# L03 - Lösningsförslag, övningsuppgifter
Lösningsförslag till övningsuppgifterna i [bilaga B](../appendix/b_exercises.md): den ifyllda
testfilen för `driver::serial::Atmega328p`.

Till skillnad från **L02** finns här ingen rättad driver, av den enkla anledningen att det inte
fanns något att rätta - se avsnittet *Inga buggar i drivern* nedan. Det som tog tid den här
gången var i stället trådarna och den mockade hårdvaran.

---

## Innehåll

```text
exercises/
└── atmega328p_test.cpp   - Ifylld testfil, hör hemma i libs/atmega/test/driver/serial/.
```

---

## Bygga och köra
Testramverket ligger i git-submodulen `libs/test`. Hämta detta först, om det inte redan är gjort:

```bash
git submodule update --init --recursive
```

Kopiera därefter in filen i övningsbiblioteket, från repots rot:

```bash
cp lectures/L03/exercises/atmega328p_test.cpp libs/atmega/test/driver/serial/atmega328p_test.cpp
```

**OBS!** Detta skriver över själva övningen. Vill ni ha tillbaka den ofärdiga testfilen,
återställ den med Git:

```bash
git restore libs/atmega/test/driver/serial/atmega328p_test.cpp
```

Kör sedan testsviten från `libs/atmega/test`:

| Kommando | Beskrivning |
|---|---|
| `make` | Bygger och kör testsviten. |
| `make build` | Bygger enbart testsviten. |
| `make run` | Bygger vid behov och kör testsviten. |
| `make clean` | Tar bort testsviten samt `yrgo::test`-biblioteket. |

Samtliga 13 testfall ska passera - de tre seriella testfallen, plus de tio som redan var gröna:

```text
13 out of 13 test cases succeeded!
```

Har ni även aktiverat GPIO-testerna från **L02** blir summan 17 i stället. Hela sviten tar
ungefär en tiondels sekund, trots att `Read`-testet innehåller en timeout på 100 ms.

---

## Skriv tester
Testfilen innehåller tre testfall:

| Testfall | Verifierar |
|---|---|
| `Initialization` | Att `isInitialized()` returnerar sant, att `setEnabled()` slår igenom på `isEnabled()` i båda riktningarna, samt att `baudRate_bps()` returnerar 9600. |
| `Transmit` | Att `printf()` går att köra mot den mockade hårdvaran, med tre trådar som spelar drivare, hårdvara och mottagare, utan att något låser sig. Hur mycket testet hinner kontrollera i praktiken - se *Vad `Transmit` faktiskt verifierar* nedan. |
| `Read` | Felhanteringen i `read()` (nullpekare, storleken 0, timeout utan inkommande data), samt att ett helt meddelande kan läsas in tecken för tecken. |

Utöver testfallen finns sex hjälpfunktioner:

| Hjälpfunktion | Roll |
|---|---|
| `initSerial()` | Hämtar singleton-instansen och aktiverar den. |
| `delay_us()` | Sover angivet antal mikrosekunder, dvs. `std::this_thread::sleep_for()`. |
| `simulateDataReg()` | Hårdvarusidan vid sändning: pollar `UDR0` och rensar `UDRE0` när registret fått ett nytt värde. |
| `printThread()` | Drivarsidan: anropar `serial.printf(msg)` och sätter därefter stoppflaggan. |
| `readDataRegThread()` | Mottagarsidan: väntar på att `UDRE0` rensas, läser `UDR0`, jämför mot förväntat tecken och sätter `UDRE0` igen. |
| `injectRxData()` | Hårdvarusidan vid mottagning: lägger ett tecken i `UDR0`, sätter `RXC0` och väntar (med tidsgräns) på att testet rensar flaggan. |

---

## Den mockade hårdvaran
Precis som med `DDRB`/`PORTB` i **L02** är `UDR0` och `UCSR0A` bara byte i en vanlig array i
testbygget, och ingenting uppdaterar dem av sig självt. Skillnaden är att en seriell överföring
har två riktningar, och att testet måste spela hårdvara i båda:

| Register/flagga | I testbygget | Betydelse |
|---|---|---|
| `UDR0` | `test::Memory::data.reg8[52]` | Dataregistret, samma register för både sändning och mottagning. |
| `UCSR0A` | `test::Memory::data.reg8[47]` | Statusregistret, innehåller flaggorna nedan. |
| `UDRE0` | bit 5 i `UCSR0A` | Satt = dataregistret är ledigt, drivern får skriva nästa byte. |
| `RXC0` | bit 7 i `UCSR0A` | Satt = ett tecken har tagits emot och kan läsas ur `UDR0`. |

Två beteenden hos riktig hårdvara finns alltså inte här, och det är precis dem testet får
efterlikna:
* **En skrivning till `UDR0` rensar inte `UDRE0`.** På riktig hårdvara sker det i samma ögonblick
  som skrivningen; här måste `simulateDataReg()` upptäcka skrivningen och rensa flaggan själv.
* **En läsning av `UDR0` rensar inte `RXC0`.** Därför rensar `Read`-testet flaggan själv efter
  varje läst tecken, vilket samtidigt är signalen till `injectRxData()` att nästa tecken kan
  matas in.

Två detaljer i drivern är värda att känna till när man läser testet:
* `print()` skickar radbrytningar som *två* tecken, `'\n'` följt av `'\r'`
  ([atmega328p.cpp:127](../../../libs/atmega/source/driver/serial/atmega328p.cpp)). Byteströmmen
  är alltså ett tecken längre än strängen.
* `read()` med en timeout pollar `RXC0` en gång per millisekund via `utils::delay_ms(1)`. Det är
  därför fallet "ingen data inom tidsgränsen" kostar sina fulla 100 ms i väggklocka - och därför
  `RXC0` måste rensas *före* det anropet, annars läser drivern det som råkar ligga kvar i `UDR0`
  sedan `Transmit`-testet.

---

## Trådarna
De tre trådarna i `Transmit` kommunicerar enbart via de virtuella registren, precis som drivern
och hårdvaran gör på riktig hårdvara. Fyra saker styr hur de är skrivna:

* **Namngivna funktioner, inte lambdas.** Trådkropparna skickas direkt till `std::thread`, med
  `std::ref`/`std::cref` för argument som ska delas. Stoppflaggan är den enda som skrivs från en
  tråd och läses från två andra, och är därför en `std::atomic<bool>`. Meddelandet är en
  `const char*` och skickas som värde - det är en pekare till en strängliteral, så det finns
  inget att referera till.
* **`printThread()` äger stoppvillkoret.** Den sätter flaggan när `printf()` returnerat, och det
  är enda anledningen till att de andra två trådarna avslutar sina loopar. Lämnas den oimplementerad
  snurrar de i stället vidare: testsviten *hänger* då i stället för att bli röd, och måste
  avbrytas med Ctrl+C.
* **`EXPECT_*` kastar undantag.** Ett undantag som tar sig ut ur en `std::thread` avslutar hela
  testprogrammet via `std::terminate` - oavsett om trådfunktionen är `noexcept` eller inte. En
  misslyckad förväntan inne i en tråd ger alltså ingen röd rad, utan en död testsvit.
* **Därför ligger `Read`-testets samtliga kontroller på huvudtråden, efter `join()`.** Tråden
  `injectRxData()` innehåller ingen enda `EXPECT_*`; den matar bara in tecken. Det är mönstret att
  ta med sig: låt trådarna spela hårdvara, och verifiera resultatet där ett fel faktiskt kan
  rapporteras.

---

## Inga buggar i drivern
Till skillnad från GPIO-drivern i **L02** innehåller `driver::serial::Atmega328p` inga
inplanterade buggar. Samtliga tre testfall blir gröna mot en helt orörd
[atmega328p.cpp](../../../libs/atmega/source/driver/serial/atmega328p.cpp), och bilaga B:s
buggjakt slutar den här gången utan en enda rättning.

Det betyder att ett rött (eller hängande) testfall i **L03** kommer från testet eller från
missförstådd mockad hårdvara, inte från drivern. Det var också där tiden gick: trådarna och
handskakningen via `UDRE0`/`RXC0`, inte felsökning av drivarkod.

---

## Vad `Transmit` faktiskt verifierar
Det här är lektionens intressantaste resultat, och värt att vara ärlig om: `Transmit` blir grönt
utan att kontrollera ett enda tecken.

Med en räknare inlagd i `readDataRegThread()` och i `simulateDataReg()` blir utfallet detsamma
körning efter körning:

```text
simulator saw 0 byte changes; reader verified 0 of 19 chars
```

Förklaringen ligger i den första punkten i avsnittet om den mockade hårdvaran. `transmitChar()`
väntar bara på att `UDRE0` ska vara *satt*
([atmega328p.cpp:29](../../../libs/atmega/source/driver/serial/atmega328p.cpp)), och eftersom en
skrivning till `UDR0` inte rensar flaggan blockerar drivern aldrig: hela strängen hinner skrivas
till `UDR0` innan `simulateDataReg()` ens hunnit varva sin loop en gång. `printThread()` sätter
då stoppflaggan, och `readDataRegThread()` bryter ut vid det allra första tecknet.

Testet är alltså ett *röktest*: det visar att `printf()` går att köra mot den mockade hårdvaran,
returnerar, och att de tre trådarna kan avslutas och joinas utan att låsa sig. Det visar
ingenting om vilka byte som skickades.

För att testet ska kunna verifiera själva byteströmmen måste mocken bete sig som hårdvaran, dvs.
rensa `UDRE0` i samma ögonblick som `UDR0` skrivs. Det kräver att `UDR0` kan upptäcka skrivningar
(i dag är den en vanlig byte i
[hw_platform.h](../../../libs/atmega/include/arch/test/hw_platform.h)) - en pollande tråd kan
aldrig vinna det tidsloppet. Så länge hårdvarusidan i stället letar efter ett *ändrat värde* i
`UDR0` kan den dessutom inte se två identiska tecken i rad, som dubbel-s:et i `"message"`.

Ett grönt testfall som inte kontrollerar något är precis den sortens testkvalitet vi återkommer
till i **L11**, då med täckningsgrad som verktyg.

---

## Vad som medvetet inte testas
* **`read()` med `timeout_ms = 0`.** Drivern läser då tills bufferten är full och väntar i en
  loop utan tidsgräns på `RXC0`. Ett sådant testfall hänger testsviten så fort tecknen inte
  matas in i exakt rätt antal, och är inte värt risken här.
* **Att `printf()` inte skickar något när drivern är avstängd.** `print()` returnerar direkt när
  `myEnabled` är falsk, men med `Transmit` i sitt nuvarande skick hade ett sådant test bara
  jämfört två tomma resultat.
* **Baud rate som verklig bittiming.** `baudRate_bps()` returnerar en konstant, och konstruktorn
  skriver 103 till `UBRR0` - båda går att läsa av, men själva överföringshastigheten på ledningen
  går inte att verifiera i ett enhetstest, varken här eller i er egen kodbas. Det hör hemma i
  HW/SW-integrationstesterna i **L17**.

---
