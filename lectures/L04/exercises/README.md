# L04 - Lösningsförslag, övningsuppgifter
Lösningsförslag till övningsuppgifterna i [bilaga B](../appendix/b_exercises.md): den ifyllda
testfilen för `driver::timer::Atmega328p`, samt drivern med de inplanterade buggarna åtgärdade.

Till skillnad från **L03**, där drivern var felfri, finns det något att hitta den här gången:
tre buggar, varav en kraschar hela testsviten.

---

## Innehåll

```text
exercises/
├── atmega328p.cpp        - Rättad driver, hör hemma i libs/atmega/source/driver/timer/.
└── atmega328p_test.cpp   - Ifylld testfil, hör hemma i libs/atmega/test/driver/timer/.
```

---

## Bygga och köra
Testramverket ligger i git-submodulen `libs/test`. Hämta detta först, om det inte redan är gjort:

```bash
git submodule update --init --recursive
```

Kopiera därefter in de två filerna i övningsbiblioteket, från repots rot:

```bash
cp lectures/L04/exercises/atmega328p.cpp      libs/atmega/source/driver/timer/atmega328p.cpp
cp lectures/L04/exercises/atmega328p_test.cpp libs/atmega/test/driver/timer/atmega328p_test.cpp
```

**OBS!** Detta skriver över själva övningen. Vill ni ha tillbaka den ofärdiga testfilen och den
buggiga drivern, återställ dem med Git:

```bash
git restore libs/atmega/source/driver/timer/atmega328p.cpp \
            libs/atmega/test/driver/timer/atmega328p_test.cpp
```

Kör sedan testsviten från `libs/atmega/test`:

| Kommando | Beskrivning |
|---|---|
| `make` | Bygger och kör testsviten. |
| `make build` | Bygger enbart testsviten. |
| `make run` | Bygger vid behov och kör testsviten. |
| `make clean` | Tar bort testsviten samt `yrgo::test`-biblioteket. |

Samtliga 15 testfall ska passera - de fem timer-testfallen, plus de tio som redan var gröna:

```text
15 out of 15 test cases succeeded!
```

Har ni även aktiverat testerna från **L02** och **L03** blir summan 22.

---

## Skriv tester
Testfilen innehåller fem testfall:

| Testfall | Verifierar |
|---|---|
| `Initialization` | Fall 1: att tre timrar kan användas samtidigt, men att en fjärde nekas - hårdvaran har bara tre kretsar. Fall 2: att en timer med timeouten 0 ms inte initieras. |
| `EnableDisable` | Att `start()`, `stop()` och `toggle()` sätter `isEnabled()` till rätt värde, och att en nyskapad timer är stoppad. |
| `Timeout` | Att `timeout_ms()` returnerar det värde konstruktorn fick, att `setTimeout_ms()` ändrar det, samt att ett anrop med 0 ms lämnar timeouten orörd. |
| `Callback` | Att callbacken körs när tillräckligt många avbrott har räknats. |
| `Restart` | Att `restart()` nollställer räknaren: efter `getMaxCount() - 1` avbrott, en omstart och ytterligare `getMaxCount() - 1` avbrott har callbacken fortfarande inte körts - men ett avbrott till utlöser den. |

Hjälpfunktionerna för callback-testningen ligger i ett eget `#ifdef CALLBACK`-block i den
ofärdiga testfilen, eftersom de används av *två* testfall:

| Hjälpfunktion | Roll |
|---|---|
| `MaxTimerCount` | Antalet timerkretsar hårdvaran har, dvs. 3. |
| `callbackInvoked` | Flagga som `testCallback()` sätter, och som testfallen läser av. |
| `resetCallbackFlag()` | Nollställer flaggan i början av varje testfall. |
| `testCallback()` | Callbacken som registreras hos timern. |
| `getMaxCount(timeout_ms)` | Räknar om en timeout i millisekunder till antal avbrott, med samma formel som drivern använder internt. |

---

## Tid utan att vänta
Det här är lektionens hela poäng, och skillnaden mot ett test som mäter riktig tid.

`driver::timer::Atmega328p` räknar avbrott i mjukvara: varje avbrott motsvarar 0,128 ms, och en
timeout inträffar när räknaren nått `myMaxCount`, dvs. timeouten omräknad till antal avbrott.
Testet behöver därför aldrig vänta - det anropar `handleCallback()` direkt, lika många gånger som
avbrottsrutinen skulle ha gjort:

| Timeout | Antal avbrott (`getMaxCount()`) | `timeout_ms()` tillbaka |
|---|---|---|
| 10 ms | 78 | 10 ms |
| 100 ms | 781 | 100 ms |
| 200 ms | 1563 | 200 ms |

Tre saker faller ut av det:
* **Testerna är exakta.** `Restart` verifierar gränsen på *ett enda avbrott*: efter
  `getMaxCount() - 1` anrop har ingenting hänt, efter ytterligare ett har callbacken körts. Det
  hade varit omöjligt att avgöra med en riktig klocka.
* **Testerna är snabba.** Hela sviten kör på en bråkdel av en sekund, trots att testfallen
  representerar hundratals millisekunder simulerad tid.
* **Testerna är deterministiska.** Ingen `sleep`, inget beroende av när operativsystemet råkar
  schemalägga tråden - och därmed inget *flaky* testfall. Jämför med `Transmit` i **L03**, som är
  beroende av hur trådarna hinner samsas, och som därför kan ge olika resultat mellan körningar.

Samma idé mötte ni i **L01**, där `driver::timer::Stub` räknade `tick()`-anrop i stället för
millisekunder. Skillnaden är att här är det den *riktiga* drivern som testas; det enda som är
simulerat är avbrottet.

---

## Buggjakt
Tre buggar finns inplanterade i
[atmega328p.cpp](../../../libs/atmega/source/driver/timer/atmega328p.cpp):

| Bugg | Var | Kort sagt |
|---|---|---|
| 1 | Konstruktorn | Reserverar en timerkrets även när timeouten är 0 ms. |
| 2 | `~Atmega328p()` | Städar upp även när initieringen misslyckades, och avrefererar då en nollpekare. |
| 3 | `setTimeout_ms()` | Stoppar timern vid timeouten 0 ms, och nollställer dessutom den timeout som redan var satt. |

### Bugg 1 - en ogiltig timer tar en krets i anspråk

```cpp
// Före:
    : myHw{Hardware::reserve()}

// Efter:
    : myHw{0U < timeout_ms ? Hardware::reserve() : nullptr}
```

Symptomet är fall 2 i initieringstestet:

```text
Test case Timer_Atmega328p.Initialization failed: EXPECT_FALSE(timer1.isInitialized()) failed: true != false (driver/timer/atmega328p_test.cpp:83)
14 out of 15 test cases succeeded!
```

En timer med timeouten 0 ms rapporterade sig som initierad, trots att den aldrig kan löpa ut
(`start()` returnerar direkt när `myMaxCount` är noll). I ett riktigt system hade den dessutom
lagt beslag på en av tre timerkretsar, och nästa timer som behövdes hade nekats utan synbar
anledning.

### Bugg 2 - destruktorn förutsätter att initieringen lyckades

```cpp
// Efter (raden som saknades):
if (!isInitialized()) { return; }
```

Med enbart den här buggen kvar syns inget rött testfall alls: testsviten skriver ut de sex gröna
testfallen före timer-testerna och dör sedan tyst, mitt i `Initialization`, när den fjärde
timern - den som inte fick någon krets - går ur scope i slutet av fall 1. Destruktorn anropar
`removeCallback()`, som går via `myHw->index` på en nollpekare.

Det är exakt samma mönster som i GPIO-drivern i **L02**, och det bilaga B pekar ut: *vad händer
med en timer som aldrig blev initierad när den går ur scope?* Ringa in kraschen på samma sätt som
då, genom att ta bort en rad i taget ur testfallet tills den försvinner.

### Bugg 3 - `setTimeout_ms(0)` raderar den timeout som fanns

```cpp
// Före:
if (0U == timeout_ms) { stop(); }
myMaxCount = maxCount(timeout_ms);

// Efter:
if (0U == timeout_ms) { return; }
myMaxCount = maxCount(timeout_ms);
```

Den gamla koden gjorde två fel på samma gång: den stoppade timern, och lät sedan
`myMaxCount = maxCount(0)` skriva över den giltiga timeouten med noll. Resultatet blev en timer
som varken var igång eller hade någon timeout kvar:

```text
Test case Timer_Atmega328p.Timeout failed: EXPECT_EQ(timer0.timeout_ms(), timeout2_ms) failed: 0 != 200 (driver/timer/atmega328p_test.cpp:147)
14 out of 15 test cases succeeded!
```

Ett ogiltigt argument ska inte tyst förstöra ett giltigt tillstånd - drivern ska ignorera anropet
och behålla den timeout den redan har.

---

## Vad som medvetet inte testas
* **De riktiga avbrotten.** `ISR(TIMER0_OVF_vect)` och dess syskon expanderar i testbygget till
  vanliga funktioner som ingen anropar. Testet går därför direkt på `handleCallback()`, vilket är
  precis det avbrottsrutinen hade gjort via `invokeCallback()`.
* **Registerinnehållet.** Att `start()`/`stop()` sätter respektive rensar rätt bit i `TIMSK0`,
  `TIMSK1` och `TIMSK2` går att verifiera på samma sätt som `DDRx`/`PORTx` i **L02**, och det är
  den naturliga utbyggnaden av `EnableDisable`. Här nöjer sig testet med `isEnabled()`.
* **Flera timrar som räknar samtidigt.** Varje testfall använder en timer i taget. Att tre timrar
  räknar oberoende av varandra är nästa steg för den som vill bygga vidare.

---
