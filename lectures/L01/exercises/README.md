# L01 - Lösningsförslag, övningsuppgifter
Lösningsförslag till övningsuppgifterna i [bilaga B](../appendix/b_exercises.md): två stubbdrivers, `driver::gpio::Stub` och
`driver::timer::Stub`, tillsammans med en testsvit skriven i `yrgo::test`.

---

## Innehåll

```text
exercises/
├── Makefile                     - Bygger och kör testsviten.
├── driver/
│   ├── gpio/
│   │   ├── interface.h          - driver::gpio::Interface (uppgift 1).
│   │   └── stub.h               - driver::gpio::Stub (uppgift 1).
│   └── timer/
│       ├── interface.h          - driver::timer::Interface (uppgift 2).
│       └── stub.h               - driver::timer::Stub (uppgift 2).
└── test/
    ├── driver/
    │   ├── gpio/
    │   │   └── stub_test.cpp    - 8 testfall, TEST(GpioStub, ...).
    │   └── timer/
    │       └── stub_test.cpp    - 16 testfall, TEST(TimerStub, ...).
    └── testsuite.cpp            - main(), som kör samtliga testfall.
```

---

## Bygga och köra
Testramverket ligger i git-submodulen `libs/test`. Hämta detta först, om det inte redan är gjort:

```bash
git submodule update --init --recursive
```

Kör därefter testsviten från den här mappen:

| Kommando | Beskrivning |
|---|---|
| `make` | Bygger och kör testsviten. |
| `make build` | Bygger enbart testsviten. |
| `make run` | Bygger vid behov och kör testsviten. |
| `make clean` | Tar bort testsviten samt `yrgo::test`-biblioteket. |

Samtliga 24 testfall ska passera:

```text
24 out of 24 test cases succeeded!
```

**OBS!** I bilagan läggs övningsmappen i repots rot, och då blir sökvägen till testramverket
`../libs/test`. Lösningsförslaget ligger i stället tre nivåer ner, så här är `YRGO_TEST_DIR`
satt till `../../../libs/test`. Det är den enda skillnaden mot makefilen i bilagan.

---

## Uppgift 1 - GPIO-stubb
`driver::gpio::Stub` lagrar pinnens nivå i en enda `bool`. Testfallen i
[test/driver/gpio/stub_test.cpp](./test/driver/gpio/stub_test.cpp):

| Testfall | Verifierar |
|---|---|
| `InitialState` | Att starttillståndet är `false` som default, och det angivna värdet när ett sådant ges. |
| `Write` | Att `write(true)` respektive `write(false)` sätter rätt nivå. |
| `Toggle` | Att `toggle()` växlar nivån, fem gånger i rad. |
| `ToggleFromEnabledState` | Att `toggle()` fungerar i båda riktningarna, även från ett aktiverat starttillstånd. |
| `RepeatedWriteKeepsState` | Att upprepade `write()` med samma värde behåller nivån, dvs. inte beter sig som en toggling. |
| `WriteAfterToggle` | Att en `write()` skriver över tillståndet från en föregående `toggle()`. |
| `InstancesAreIndependent` | Att två stubbar inte delar tillstånd. |
| `UsageViaInterface` | Att stubben går att använda via en `driver::gpio::Interface`-referens. |

De tre första motsvarar det bilagan efterfrågar. De övriga fem är tillagda för att täcka in
misstag som annars slinker igenom: en `toggle()` som sätter i stället för att växla, en `write()`
som ignorerar sitt argument, ett tillstånd som råkat bli delat mellan instanser, samt att
polymorfin via interfacet faktiskt fungerar.

---

## Uppgift 2 - Timer-stubb
`driver::timer::Stub` mäter ingen verklig tid. I stället räknar den `tick()`-anrop, där varje
anrop motsvarar 1 ms, och jämför räknaren mot den timeout som angavs vid konstruktion. Ett test
som vill simulera 100 ms anropar alltså `tick()` 100 gånger.

Tre detaljer i beteendet är värda att hålla reda på när man läser testerna:
* `hasTimedOut()` **nollställer räknaren** när den rapporterar en timeout. Timern börjar därmed
  om av sig själv, precis som en hårdvaruflagga som töms i samma stund som den läses. Det är
  också därför räknaren är `mutable`: metoden är `const` utåt sett, men ändrar räknaren internt.
* `start()` och `toggle()` nollställer räknaren, medan `stop()` bara fryser den. En timeout som
  redan hunnit inträffa finns alltså kvar efter ett `stop()`.
* `tick()` räknar bara upp så länge räknaren ännu inte nått timeouten. Räknaren kan därför aldrig
  passera sitt timeout-värde, på samma sätt som en hårdvaruräknare som stannar vid sitt
  jämförvärde.

Testfallen i [test/driver/timer/stub_test.cpp](./test/driver/timer/stub_test.cpp):

| Testfall | Verifierar |
|---|---|
| `InitialState` | Att en ny timer är stoppad, rapporterar sin timeout och inte har timat ut. |
| `InitialStateWhenStarted` | Att konstruktorns andra parameter styr om timern är igång direkt. |
| `StartAndStop` | Att `start()` och `stop()` sätter `isRunning()` till rätt värde. |
| `Toggle` | Att `toggle()` växlar `isRunning()`, fem gånger i rad. |
| `TickWhileStopped` | Att `tick()` inte har någon verkan på en stoppad timer, hur många gånger det än anropas. |
| `NoTimeoutBeforeConfiguredTicks` | Att timern *inte* har timat ut efter `timeout_ms() - 1` tick. |
| `TimeoutAfterConfiguredTicks` | Att timern har timat ut efter exakt `timeout_ms()` tick. |
| `TimeoutIsClearedWhenRead` | Att ett andra `hasTimedOut()` direkt efter det första returnerar `false`. |
| `TimeoutIsPeriodic` | Att timern timar ut igen efter ytterligare `timeout_ms()` tick, utan omstart. |
| `StartClearsElapsedTime` | Att `start()` nollställer räknaren mitt i en pågående mätning. |
| `ToggleClearsElapsedTime` | Att `toggle()` nollställer räknaren på samma sätt. |
| `ElapsedTimeSaturatesAtTimeout` | Att räknaren stannar vid timeouten och inte slår runt. |
| `StopKeepsReachedTimeout` | Att `stop()` fryser räknaren, så en redan nådd timeout finns kvar. |
| `MinimumTimeout` | Att en timer med timeouten 1 ms timar ut på ett enda tick. |
| `InstancesAreIndependent` | Att två timrar inte delar vare sig räknare, timeout eller körstatus. |
| `UsageViaInterface` | Att stubben går att använda via en `driver::timer::Interface`-referens. |

Paret `NoTimeoutBeforeConfiguredTicks` och `TimeoutAfterConfiguredTicks` är testsvitens
viktigaste, precis som bilagan påpekar: det är i gränsen mellan dem ett *off-by-one*-fel hade
gömt sig. Att de är uppdelade i två testfall i stället för ett är avsiktligt - misslyckas bygget
säger testfallets namn direkt åt vilket håll felet ligger.

`ElapsedTimeSaturatesAtTimeout` tickar en hel varvlängd för räknaren, 65 536 gånger, utan att
läsa av timeouten däremellan. Det låter i överkant, men är just vad som krävs för att skilja en
räknare som stannar vid timeouten från en som räknar vidare: eftersom räknaren är en
`std::uint16_t` slår en oskyddad räknare runt till noll efter exakt så många tick, och den
inträffade timeouten hade då försvunnit. Med färre tick beter sig de två varianterna likadant,
och testet hade passerat även med buggen kvar.

### Ett fall som medvetet inte testas
Konstruktorn vägrar skapa en timer med timeouten noll, och avbryter i så fall hela processen via
`std::terminate()`. Något sådant testfall finns därför inte i sviten: `yrgo::test` har inget stöd
för att fånga upp ett program som avslutar sig självt, så testet hade tagit med sig resten av
testsviten i fallet.

---
