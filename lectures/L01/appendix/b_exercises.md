# Bilaga B - Övningsuppgifter
Ni har redan hårdvara och en egen kodbas från **P02** sedan tidigare, men dessa
övningsuppgifter görs medvetet utan att röra vare sig hårdvaran eller er egen kodbas. Syftet är
att öva på själva tekniken i ett litet, isolerat format innan ni tillämpar den på riktigt.

Ni ska skapa två små, självständiga stubbdrivers som efterliknar hårdvara med vanliga
medlemsvariabler. Ni ska sedan skriva enhetstester för dessa. Samma princip använder ni sedan
mot era egna stubbar från **P02** i **L02–L04**.

**OBS!** Namnrymden `driver::` är densamma som i övningsbiblioteket och i **P02**, men
interfacen ni bygger här är kraftigt förenklade övningsvarianter, och har alltså *inte* samma
metoder som de riktiga `driver::gpio::Interface` och `driver::timer::Interface`. Det är
principen, inte API:et, som ska kännas igen. Eftersom övningen ligger i en helt egen mapp, med
en egen testsvit, krockar den inte med någon befintlig kod. De riktiga `Esp32s3`-drivers, som
anropar ESP-IDF, kräver i stället mockningstekniken från **L06** för att kunna enhetstestas,
något ni tillämpar fullt ut när ni skriver tester för er riktiga kodbas i **P04**.

---

## Förberedelser
Testramverket `yrgo::test` ligger i en git-submodul (`libs/test`) och följer inte med vid en
vanlig kloning. Kontrollera därför att submodulen är hämtad innan ni börjar:

```bash
git submodule update --init --recursive
```

Är mappen `libs/test` tom avbryts bygget nedan med `No rule to make target 'lib'`.

Testramverket inkluderas sedan med `#include "yrgo/test/test.h"` i varje testfil. Makefilen
lägger till `../libs/test/include` i sökvägen, så ingen längre sökväg behövs.

---

## Övningsmappen
Skapa en liten mapp för övningen, t.ex. `unit_test_exercise/`. Lägg den direkt i repots rot, så
att sökvägen till testramverket blir `../libs/test`. Mappen får följande innehåll:

```text
unit_test_exercise/
├── Makefile                     - Bygger och kör testsviten (se nedan).
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
    │   │   └── stub_test.cpp    - Testfall TEST(GpioStub, ...).
    │   └── timer/
    │       └── stub_test.cpp    - Testfall TEST(TimerStub, ...).
    └── testsuite.cpp            - main(), som kör samtliga testfall.
```

Upplägget är detsamma som i
[ATmega328p-övningsbiblioteket](../../../libs/atmega/README.md): varje driver har en
`interface.h` och en `stub.h` under `driver/<drivernamn>/`, och testerna ligger separat under
`test/`, i en mappstruktur som speglar drivernas. Precis som i tidigare kurser definierar vi
stubbarna direkt i headerfilerna. Eftersom övningsmappen själv ligger i inkluderingssökvägen
(`-I.` i makefilen) inkluderas headerfilerna med sin fullständiga sökväg, alltså
`driver/gpio/stub.h` snarare än bara `stub.h`.

Filen `test/testsuite.cpp` innehåller bara programmets `main()`:

```cpp
/**
 * @brief Test suite for the unit test exercise.
 */
#include "yrgo/test/test.h"

/**
 * @brief Run all test cases.
 *
 * @return 0 on success, or -1 on failure.
 */
int main() { return yrgo::test::runAllTests() ? 0 : -1; }
```

### Makefile
Kopiera makefilen nedan till `unit_test_exercise/Makefile`. Den bygger `yrgo::test`-biblioteket
i `../libs/test` och länkar testsviten mot det, enligt samma upplägg som
[libs/atmega/test/Makefile](../../../libs/atmega/test/Makefile) (se även
[libs/test](../../../libs/test/README.md#integration-as-a-git-submodule)):

```makefile
# Testsuite target.
TARGET := testsuite

# Test files - update this list as new test files are added to the exercise.
TEST_FILES := test/driver/gpio/stub_test.cpp \
              test/driver/timer/stub_test.cpp \
              test/testsuite.cpp \

# Directory containing the yrgo::test framework.
YRGO_TEST_DIR := ../libs/test
YRGO_TEST_LIB := $(YRGO_TEST_DIR)/libyrgo_test.a

# C++ compiler.
CXX_COMPILER = g++

# C++ compiler flags.
CXX_FLAGS = -std=c++17 -Werror -Wall -I. -I$(YRGO_TEST_DIR)/include

.PHONY: default build run clean

# Build and run the test suite as default:
default: build run

# Build the yrgo::test library.
$(YRGO_TEST_LIB):
	@$(MAKE) -C $(YRGO_TEST_DIR) lib

# Build the test suite.
build: $(YRGO_TEST_LIB)
	@$(CXX_COMPILER) $(TEST_FILES) -o $(TARGET) $(CXX_FLAGS) -L$(YRGO_TEST_DIR) -lyrgo_test

# Run the test suite.
run: build
	@./$(TARGET)

# Clean the test suite and the yrgo::test library.
clean:
	@rm -f $(TARGET)
	@$(MAKE) -C $(YRGO_TEST_DIR) clean
```

**OBS!** Raderna under respektive mål måste börja med ett tab-tecken, inte med mellanslag.
Lägg till fler testfiler i `TEST_FILES` allt eftersom ni skapar dem.

Kör sedan övningen från mappen `unit_test_exercise/`:

| Kommando | Beskrivning |
|---|---|
| `make` | Bygger och kör testsviten. |
| `make build` | Bygger enbart testsviten. |
| `make run` | Bygger vid behov och kör testsviten. |
| `make clean` | Tar bort testsviten samt `yrgo::test`-biblioteket. |

### Kodkonventioner
Följ samma konventioner i båda uppgifterna:
* Varje headerfil inleds med `#pragma once`.
* Interface och stub för en driver ligger i namnrymden `driver::<drivernamn>`, t.ex.
  `driver::gpio`.
* Interfacet har en publik, virtuell destruktor. Övriga metoder är rent virtuella.
* Alla metoder markeras `noexcept`. Metoder som bara läser ut ett tillstånd är dessutom `const`
  och markeras `[[nodiscard]]`, eftersom ett anrop vars returvärde kastas bort är meningslöst.
* Stubben märks `final`, ärver interfacet publikt och implementerar metoderna med `override`.
  Även dess destruktor deklareras `override` och `default`.
* Stubbarna ska varken gå att kopiera eller flytta. Markera därför samtliga fyra operationer
  som `= delete`: kopieringskonstruktorn, flyttkonstruktorn, kopieringstilldelningen samt
  flytttilldelningen. En driver representerar en fysisk resurs, och två kopior av samma pin
  eller timer vore meningslöst.
* Default-konstruktorn hanteras olika i de två uppgifterna, se respektive uppgift nedan.

---

## Uppgift 1 - GPIO-stubb

### Skapa interfacet och stubben
Skapa ett interface `driver::gpio::Interface` i `driver/gpio/interface.h` med
metoderna:
* `write(bool high)`: sätter pinnens logiska nivå.
* `read() const`: läser pinnens nuvarande logiska nivå.
* `toggle()`: togglar pinnens nuvarande logiska nivå.

Skapa därefter en klass `driver::gpio::Stub` i `driver/gpio/stub.h` som ärver interfacet:
* I stället för att använda en riktig pin lagrar stubben bara ett bool-värde internt.
* Konstruktorn är `explicit` och tar pinnens starttillstånd som argument. Ge parametern
  defaultargumentet `false`, så att en stub som skapas utan argument börjar som låg.
* Stubben ska varken gå att kopiera eller flytta. Radera därför kopierings- och
  flyttkonstruktorerna samt kopierings- och flytttilldelningen med `= delete`.
* Radera däremot **inte** default-konstruktorn här. Konstruktorns defaultargument gör redan
  `Stub{}` giltigt, och en raderad `Stub()` vid sidan av den skulle bara göra anropet tvetydigt
  i stället för att förbjuda det.

### Skriv enhetstester
Skriv enhetstester (`TEST(GpioStub, ...)`) i `test/driver/gpio/stub_test.cpp` som verifierar
bland annat:
* Att en nyskapad stubb har rätt starttillstånd.
* Att `write(true)` respektive `write(false)` sätter pinnen till rätt nivå.
* Att `toggle()` växlar tillståndet, och att två `toggle()`-anrop i rad återställer det
  ursprungliga tillståndet.

---

## Uppgift 2 - Timer-stubb

### Skapa interfacet
Skapa filen `driver/timer/interface.h`, med ett interface `Interface` i namnrymden
`driver::timer`. Filen behöver `<cstdint>`, eftersom timeouten anges som en `std::uint16_t`.
Utöver destruktorn ska interfacet deklarera sju metoder:
* `timeout_ms()`: returnerar timerns timeout i millisekunder.
* `isRunning()`: returnerar `true` om timern är igång.
* `start()`: startar timern.
* `stop()`: stoppar timern.
* `toggle()`: växlar mellan igång och stoppad.
* `tick()`: räknar upp timern ett steg, motsvarande 1 ms.
* `hasTimedOut()`: returnerar `true` om timern har timat ut.

Tre av dem, `timeout_ms()`, `isRunning()` och `hasTimedOut()`, läser bara ut ett tillstånd och
ska därför vara `const` och `[[nodiscard]]` enligt [kodkonventionerna](#kodkonventioner) ovan.

### Skapa stubben
Skapa filen `driver/timer/stub.h`, med en klass `Stub` som inkluderar och ärver interfacet.
Utöver `driver/timer/interface.h` behöver filen tre standardheaders: `<cstdint>` för
`std::uint16_t`, `<cstdio>` för `std::fprintf` samt `<exception>` för `std::terminate`.

Eftersom vi varken vill eller kan vänta in riktig tid i ett enhetstest mäter stubben ingen
verklig tid, utan efterliknar en timer med tre medlemsvariabler:
* Timeouten i millisekunder. Den sätts en gång vid konstruktion och ändras aldrig, så
  deklarera den `const`.
* En räknare för antalet `tick()`-anrop sedan senaste nollställning.
* En flagga som anger om timern är igång.

Konstruktorn ska vara `explicit` och ta två parametrar: timeouten i millisekunder, samt en
flagga som anger om timern ska vara igång direkt. Den andra parametern ges ett defaultargument,
så att en timer som inte anges närmare skapas stoppad. Initiera samtliga medlemsvariabler i
konstruktorns initieringslista.

Konstruktorn ska dessutom vägra skapa en timer utan timeout. Kontrollera därför timeouten först
i konstruktorns kropp, med konstanten först i jämförelsen (`0U == timeout_ms`): är timeouten
noll skrivs felmeddelandet `Cannot create timer without a timeout!` ut på `stderr` via
`std::fprintf()`, följt av ett anrop till `std::terminate()` som avbryter programmet. En timer med
timeout noll skulle ju ha timat ut redan innan den startats, vilket aldrig kan vara avsikten.

Timer-stubben ska varken gå att kopiera eller flytta. Radera därför kopierings- och
flyttkonstruktorerna samt kopierings- och flytttilldelningen med `= delete`. Till skillnad från
GPIO-stubben ska här **även** default-konstruktorn raderas: timeouten måste alltid anges, så en
timer utan argument ska inte gå att skapa.

Metoderna implementerar timerns beteende:
* `start()`: startar timern **och** nollställer räknaren, så att timern alltid börjar räkna om
  från noll.
* `stop()`: stoppar timern, men lämnar räknaren orörd.
* `toggle()`: växlar mellan igång och stoppad, och nollställer räknaren.
* `tick()`: räknar upp räknaren med ett - men bara om timern är igång *och* räknaren ännu inte
  har nått timeouten. Räknaren kan alltså aldrig passera sitt timeout-värde, precis som en
  hårdvaruräknare som stannar när den träffar sitt jämförvärde.
* `hasTimedOut()`: returnerar `true` när räknaren har nått timeouten, dvs. när `tick()` har
  anropats minst `timeout_ms()` gånger sedan räknaren senast nollställdes. Vid en timeout ska
  räknaren nollställas direkt, så att timern automatiskt börjar om - precis som en
  hårdvaruflagga som töms i samma stund som den läses.

Att `hasTimedOut()` både är `const` och ändrar räknaren går bara ihop om räknaren deklareras
`mutable`. Det är avsiktligt: utåt sett *läser* metoden bara timerns tillstånd, medan
nollställningen är en intern detalj.

Vill ett test simulera att t.ex. 100 ms har förflutit anropas `tick()` 100 gånger i en loop.

### Skriv enhetstester
Skriv enhetstester (`TEST(TimerStub, ...)`) i `test/driver/timer/stub_test.cpp` som verifierar
bland annat:
* Att en nyskapad timer är stoppad, rapporterar den timeout som angavs vid konstruktion och
  inte har timat ut, samt att en timer som konstrueras som igång också rapporteras som igång.
* Att `start()`, `stop()` och `toggle()` sätter `isRunning()` till rätt värde.
* Att `tick()`-anrop på en stoppad timer aldrig får timern att tima ut.
* Att timern har timat ut efter exakt `timeout_ms()` stycken `tick()`-anrop, men *inte* ett
  tick tidigare. Detta är övningens viktigaste testfall, eftersom det är precis här ett
  *off-by-one*-fel hade smugit sig in.
* Att ett andra anrop till `hasTimedOut()`, direkt efter det första, returnerar `false`,
  eftersom räknaren nollställdes vid avläsningen - och att timern timar ut igen efter
  ytterligare `timeout_ms()` stycken `tick()`-anrop.
* Att `start()` och `toggle()` nollställer räknaren, medan `stop()` bara fryser den.

Skriv däremot inget testfall som skapar en timer med timeouten noll. Konstruktorn avbryter då
hela processen via `std::terminate`, och tar med sig resten av testsviten i fallet -
`yrgo::test` har inget stöd för att fånga upp ett program som avslutar sig självt.

---
