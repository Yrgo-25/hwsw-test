# L08 - Lösningsförslag, övningsuppgifter
Lösningsförslag till övningsuppgiften i [bilaga B](../appendix/b_exercises.md): en färdig
`driver::tempsensor::Stub` för ATmega328p-övningsbiblioteket, samt svar på diskussionsfrågorna.

Till skillnad från **L02–L04** är det den här gången inte en testfil som ska fyllas i, utan själva
testverktyget: en stubb som komponenttestet för `logic::Logic` använder i **L11**.

---

## Innehåll

```text
exercises/
└── stub.h   - Färdig temperatursensorstubb, hör hemma i libs/atmega/include/driver/tempsensor/.
```

---

## Bygga och köra
Testramverket ligger i git-submodulen `libs/test`. Hämta detta först, om det inte redan är gjort:

```bash
git submodule update --init --recursive
```

Kopiera därefter in stubben i övningsbiblioteket, från repots rot:

```bash
cp lectures/L08/exercises/stub.h libs/atmega/include/driver/tempsensor/stub.h
```

**OBS!** Detta skriver över själva övningen. Vill ni ha tillbaka den tomma stubben, återställ den
med Git:

```bash
git restore libs/atmega/include/driver/tempsensor/stub.h
```

Kör sedan testsviten från `libs/atmega/test`:

| Kommando | Beskrivning |
|---|---|
| `make` | Bygger och kör testsviten. |
| `make build` | Bygger enbart testsviten. |
| `make run` | Bygger vid behov och kör testsviten. |
| `make clean` | Tar bort testsviten samt `yrgo::test`-biblioteket. |

Stubben har ingen egen testfil, så antalet testfall ändras inte. Samtliga tio testfall som redan
var gröna ska fortfarande passera:

```text
10 out of 10 test cases succeeded!
```

Har ni även aktiverat testerna från **L02–L04** blir summan 22. Att sviten fortfarande bygger är
ändå en kontroll i sig: stubben måste gå att kompilera mot interfacet, annars stannar bygget.

---

## Stubbens delar
`driver::tempsensor::Interface` har bara två metoder. Stubben implementerar dem, och lägger till
två hjälpmetoder som bara testet använder:

| Medlem | Del av | Roll |
|---|---|---|
| `Stub(initialTemp = 25)` | Stubben | Skapar en initierad sensor med given starttemperatur. |
| `isInitialized()` | Interfacet | Returnerar om sensorn är initierad. |
| `read()` | Interfacet | Returnerar den temperatur testet har satt, i grader Celsius. |
| `setTemp(temp)` | Stubben | Sätter temperaturen som `read()` returnerar (virtuell indata). |
| `setInitialized(initialized)` | Stubben | Simulerar en sensor som inte gick att initiera. |

Som i övriga stubbar i biblioteket (se t.ex.
[adc/stub.h](../../../libs/atmega/include/driver/adc/stub.h)) är klassen `final`, konstruktorn
`explicit`, samtliga metoder `noexcept`, och kopiering samt flytt är borttagna.

---

## Varför just de här hjälpmetoderna?
En stubb är inte till för att testas, utan för att testa *med*. Hjälpmetoderna finns för att
komponenttestet ska kunna styra `Logic`s omvärld och veta vilket resultat som är det rätta:
* **`setTemp()`** gör temperaturen känd i förväg. `Logic` läser sensorn när temperaturknappen
  trycks ned och när temperaturtimern löper ut, och skriver ut värdet via serial. Med en känd
  temperatur, t.ex. 25 grader, kan testet verifiera exakt vad som skrevs ut. Testfallet
  `TempHandling` i [logic_test.cpp](../../../libs/atmega/test/logic/logic_test.cpp) gör just
  detta i **L11**.
* **`setInitialized()`** gör felfallet testbart. `Logic::isInitialized()` kräver att *samtliga*
  drivers är initierade, annars startar systemet aldrig. Med en stubb som kan simulera en trasig
  sensor går det att verifiera det beteendet, utan att någon ska behöva koppla ur en riktig
  sensor.

---

## Designval att känna igen
* **Default-värde för konstruktorparametern.** `logic_test.cpp` skapar samtliga stubbar helt
  utan argument (`tempSensor{}`). En stubb med enbart `explicit Stub(int16_t)` hade klarat sig
  här, men stoppat bygget av komponenttestet i **L11**.
* **Initierad från start.** En nyskapad stubb ska bete sig som en fungerande sensor. Felfallet är
  något testet väljer aktivt, inte stubbens utgångsläge. Samma princip gäller för `gpio`- och
  `timer`-stubbarna i **L09**: utgångsläget ska motsvara den riktiga drivrutinens.
* **`int16_t`, inte `double`.** Typen bestäms av interfacet, som i sin tur följer
  `driver::tempsensor::Tmp36`: temperaturen avrundas till hela grader. Stubben ska inte vara mer
  exakt än det interface den ersätter.
* **Ingen validering i `setTemp()`.** Till skillnad från `adc::Stub::setValue()`, som avvisar
  värden över ADC:ns maxvärde, finns ingen naturlig gräns här: en temperatur på -40 eller 125
  grader är rimlig för en sensor. Att låta testet sätta även extrema värden är dessutom precis
  vad som behövs för att testa gränsfall i **L12**.

---

## Diskussionsfrågorna
**Vad testar ett komponenttest som ett enhetstest inte fångar, och tvärtom?**
Ett komponenttest fångar fel i *samspelet*: att `Logic` läser sensorn vid rätt tillfälle, skickar
värdet vidare till serial och startar om temperaturtimern. Varje del kan vara korrekt för sig och
ändå kopplas ihop fel. Ett enhetstest fångar i stället fel *inuti* en enskild klass, t.ex. en
felaktig temperaturformel i `Tmp36`, och pekar ut exakt var felet ligger. Ett komponenttest som
failar säger bara att något i flödet gick fel.

**Vad skulle hända med testets hastighet och determinism om `Logic` använde den riktiga
temperatursensorn?**
Testet skulle kräva fysisk hårdvara, och resultatet skulle bero på rummets temperatur. Det
förväntade värdet vore inte känt i förväg, så testet skulle antingen behöva en generös tolerans
eller bli *flaky*, dvs. ibland grönt och ibland rött utan att koden ändrats. Med en stubb är
temperaturen exakt den testet har bestämt, varje gång.

---

## Vad som medvetet inte ingår
* **En egen testfil för stubben.** Till skillnad från `gpio`- och `timer`-stubbarna i **L09**
  har `tempsensor::Stub` inga egna tester. Logiken är så enkel att den i praktiken testas
  indirekt av komponenttestet i **L11**. Den som vill kan ändå skriva ett par egna testfall, och
  lägga till filen i `TEST_FILES` i [makefilen](../../../libs/atmega/test/Makefile).
* **Komponenttestet för `Logic`.** `logic_test.cpp` går att bygga först när även `gpio`- och
  `timer`-stubbarna är klara, och fylls därför i under **L11**.

---
