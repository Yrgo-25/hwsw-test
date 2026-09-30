# L09 - Lösningsförslag, övningsuppgifter
Lösningsförslag till övningsuppgiften i [bilaga A](../appendix/a_exercises.md): en färdig
`driver::gpio::Stub` samt en färdig `driver::timer::Stub` för ATmega328p-övningsbiblioteket, samt
svar på diskussionsfrågorna.

Precis som i **L08** är det inte en testfil som ska fyllas i, utan själva testverktyget. Den här
gången finns dock färdiga stubbtester, som ska bli gröna. Tillsammans med `tempsensor::Stub` från
**L08** är det de här stubbarna som komponenttestet för `logic::Logic` använder i **L11**.

---

## Innehåll

```text
exercises/
├── gpio/
│   └── stub.h   - Färdig GPIO-stubb, hör hemma i libs/atmega/include/driver/gpio/.
└── timer/
    └── stub.h   - Färdig timerstubb, hör hemma i libs/atmega/include/driver/timer/.
```

---

## Bygga och köra
Testramverket ligger i git-submodulen `libs/test`. Hämta detta först, om det inte redan är gjort:

```bash
git submodule update --init --recursive
```

Kopiera därefter in stubbarna i övningsbiblioteket, från repots rot:

```bash
cp lectures/L09/exercises/gpio/stub.h libs/atmega/include/driver/gpio/stub.h
cp lectures/L09/exercises/timer/stub.h libs/atmega/include/driver/timer/stub.h
```

**OBS!** Detta skriver över själva övningen. Vill ni ha tillbaka de tomma stubbarna, återställ dem
med Git:

```bash
git restore libs/atmega/include/driver/gpio/stub.h libs/atmega/include/driver/timer/stub.h
```

Ta sedan bort `#ifdef LECTURE9` samt tillhörande `#endif` i
[driver/gpio/stub_test.cpp](../../../libs/atmega/test/driver/gpio/stub_test.cpp) och
[driver/timer/stub_test.cpp](../../../libs/atmega/test/driver/timer/stub_test.cpp), och kör
testsviten från `libs/atmega/test`:

| Kommando | Beskrivning |
|---|---|
| `make` | Bygger och kör testsviten. |
| `make build` | Bygger enbart testsviten. |
| `make run` | Bygger vid behov och kör testsviten. |
| `make clean` | Tar bort testsviten samt `yrgo::test`-biblioteket. |

Stubbtesterna tillför elva testfall, fem för `gpio::Stub` och sex för `timer::Stub`. Tillsammans
med de tio som redan var gröna ska samtliga passera:

```text
21 out of 21 test cases succeeded!
```

Har ni även aktiverat testerna från **L02–L04** blir summan 33.

---

## Stubbarnas delar
Båda stubbarna implementerar sina respektive interface, och lägger till ett par hjälpmetoder som
bara testerna använder.

### `driver::gpio::Stub`

| Medlem | Del av | Roll |
|---|---|---|
| `Stub(mode = Mode::Input)` | Stubben | Skapar en initierad GPIO i given mode, låg och utan avbrott. |
| `isInitialized()` | Interfacet | Returnerar om GPIO:n är initierad. |
| `mode()` | Interfacet | Returnerar den mode som angavs vid skapandet. |
| `read()` | Interfacet | Returnerar aktuellt tillstånd (hög/låg). |
| `write(output)` | Interfacet | Sätter tillståndet, oavsett mode (virtuell indata för ingångar). |
| `toggle()` | Interfacet | Inverterar tillståndet. |
| `enableInterrupt(enable)` | Interfacet | Aktiverar/avaktiverar avbrott för pinnen. |
| `enableInterruptOnPort(enable)` | Interfacet | Aktiverar/avaktiverar avbrott för porten. |
| `isInterruptEnabled()` | Stubben | Returnerar om avbrott är aktiverade (pin eller port). |
| `setInitialized(initialized)` | Stubben | Simulerar en GPIO som inte gick att initiera. |

### `driver::timer::Stub`

| Medlem | Del av | Roll |
|---|---|---|
| `Stub(timeout_ms = 1000U)` | Stubben | Skapar en initierad, stoppad timer med given timeout. |
| `isInitialized()` | Interfacet | Returnerar om timern är initierad. |
| `isEnabled()` | Interfacet | Returnerar om timern är igång. |
| `hasTimedOut()` | Interfacet | Returnerar om timern har löpt ut, dvs. det testet har satt. |
| `timeout_ms()` | Interfacet | Returnerar timeouten i millisekunder. |
| `setTimeout_ms(timeout_ms)` | Interfacet | Sätter en ny timeout. |
| `start()` / `stop()` / `toggle()` | Interfacet | Startar, stoppar respektive växlar timern; nollställer timeout. |
| `restart()` | Interfacet | Samma som `start()`, eftersom stubben saknar räknare. |
| `setTimedOut(timedOut)` | Stubben | Simulerar att timern har löpt ut (virtuell indata). |
| `setInitialized(initialized)` | Stubben | Simulerar en timer som inte gick att initiera. |

Som i övriga stubbar i biblioteket (se t.ex.
[adc/stub.h](../../../libs/atmega/include/driver/adc/stub.h)) är klasserna `final`, konstruktorerna
`explicit`, samtliga metoder `noexcept`, och kopiering samt flytt är borttagna. I båda stubbarna
ignoreras anrop som ändrar tillståndet när stubben inte är initierad, precis som i de riktiga
drivrutinerna.

---

## Varför just de här hjälpmetoderna?
En stubb är inte till för att testas, utan för att testa *med*. Hjälpmetoderna finns för att
komponenttestet ska kunna styra `Logic`s omvärld och se vad `Logic` gjorde med den:
* **`timer::Stub::setTimedOut()`** ersätter verklig tid. Stubben har ingen räknare, så tiden står
  still tills testet bestämmer att timern har löpt ut. Testfallet `DebounceHandling` i
  [logic_test.cpp](../../../libs/atmega/test/logic/logic_test.cpp) simulerar på så vis att
  debounce-timern löper ut, utan att vänta en enda millisekund.
* **`gpio::Stub::isInterruptEnabled()`** gör ett dolt tillstånd synligt. Interfacet kan slå på och
  av avbrott, men inte läsa av dem. Utan hjälpmetoden går det inte att verifiera att `Logic` stänger
  av knapparnas avbrott under debounce-tiden och slår på dem igen efteråt.
* **`setInitialized()`** gör felfallet testbart, precis som i `tempsensor::Stub`.
  `Logic::isInitialized()` kräver att *samtliga* drivers är initierade, annars startar systemet
  aldrig.

GPIO-stubben behöver däremot ingen egen metod för virtuell indata: `write()` fungerar även på en
ingång, och simulerar då en knapptryckning. `logic_test.cpp` trycker ned knapparna på just det
sättet.

---

## Designval att känna igen
* **Default-värden för konstruktorparametrarna.** Stubbtesterna skapar stubbarna med argument
  (`gpio::Stub{gpio::Mode::Input}`, `timer::Stub{100U}`), medan `logic_test.cpp` skapar dem helt
  utan argument. Båda måste gå att bygga.
* **Utgångsläget motsvarar den riktiga drivrutinens.** `timer::Stub` startar *inte* i
  konstruktorn, eftersom `driver::timer::Atmega328p` inte heller gör det om det inte uttryckligen
  begärs. En timer som startar sig själv gör att `Logic` senare *växlar av* toggle-timern när
  testet förväntar sig att den slås på, och felet ser då ut att ligga i `Logic`. På samma sätt är
  en nyskapad GPIO låg och utan aktiverade avbrott.
* **`write()` fungerar oavsett mode.** Den riktiga `driver::gpio::Atmega328p` ignorerar `write()`
  på en ingång. Stubben gör inte det, eftersom det är så testet sätter en knapps tillstånd. Här
  avviker stubben medvetet från den riktiga drivrutinen, till förmån för testbarheten.
* **En gemensam avbrottsflagga för pin och port.** I den riktiga drivrutinen styr
  `enableInterrupt()` pinnens avbrottsmask (`PCMSK`) och `enableInterruptOnPort()` hela portens
  (`PCICR`). Stubben förenklar detta till en enda flagga, som speglar det senaste anropet. Det
  räcker för `Logic`, som slår på pinnens avbrott en gång i konstruktorn och därefter bara växlar
  portens.
* **Ett oinitierat tillstånd är ett nollställt tillstånd.** `setInitialized(false)` sätter GPIO:n
  låg och stänger av avbrotten, respektive stoppar timern. En trasig drivrutin ska inte råka se ut
  att vara igång.

---

## Diskussionsfrågorna
**Vilka hjälpmetoder lade ni till utöver interfacet, och varför behövs de i ett test?**
Hjälpmetoderna faller i två grupper. Några *styr* omvärlden: `setTimedOut()` och `setInitialized()`
skapar situationer som annars kräver verklig tid eller trasig hårdvara. Andra *avläser* den:
`isInterruptEnabled()` visar ett tillstånd som interfacet inte exponerar. Utan dem kan testet
varken framkalla de händelser `Logic` ska reagera på, eller se hur `Logic` reagerade.

**Varför är det värt att ha egna tester för stubbarna, när de ändå bara är testverktyg?**
Ett komponenttest är aldrig bättre än sina stubbar. En felaktig stubb gör att komponenttestet
failar, och felet ser då ut att ligga i `Logic`, som i exemplet med timern som startar sig själv.
Omvänt kan en felaktig stubb få ett trasigt `Logic` att se korrekt ut. Stubbtesterna slår fast att
verktyget fungerar innan det används, så att ett rött komponenttest i **L11** pekar på rätt ställe.

---

## Vad som medvetet inte ingår
* **Separata avbrottsflaggor för pin och port.** Den som vill spegla den riktiga drivrutinen kan
  lägga till en egen flagga för porten, med t.ex. en `isPortInterruptEnabled()`, och låta
  `isInterruptEnabled()` kräva att *båda* är aktiverade. Testet `Gpio_Stub.Interrupt` behöver i så
  fall uppdateras.
* **En räknare i timerstubben.** Stubben simulerar inte tid, och `restart()` gör därför samma sak
  som `start()`. Att låta testet bestämma när timern löper ut är precis det som gör testet snabbt
  och deterministiskt.
* **Komponenttestet för `Logic`.** Med samtliga stubbar på plats går
  [logic_test.cpp](../../../libs/atmega/test/logic/logic_test.cpp) att bygga, men det fylls i under
  **L11**.

---
