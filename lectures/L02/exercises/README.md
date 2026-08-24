# L02 - Lösningsförslag, övningsuppgifter
Lösningsförslag till övningsuppgifterna i [bilaga B](../appendix/b_exercises.md): den ifyllda
testfilen för `driver::gpio::Atmega328p`, samt drivern med de inplanterade buggarna åtgärdade.

---

## Innehåll

```text
exercises/
├── atmega328p.cpp        - Rättad driver, hör hemma i libs/atmega/source/driver/gpio/.
└── atmega328p_test.cpp   - Ifylld testfil, hör hemma i libs/atmega/test/driver/gpio/.
```

Till skillnad från **L01** finns här ingen egen mappstruktur eller Makefile. Filerna hör hemma i
[ATmega328p-övningsbiblioteket](../../../libs/atmega/README.md) och byggs tillsammans med resten
av testsviten där.

---

## Bygga och köra
Testramverket ligger i git-submodulen `libs/test`. Hämta detta först, om det inte redan är gjort:

```bash
git submodule update --init --recursive
```

Kopiera därefter in de två filerna i övningsbiblioteket, från repots rot:

```bash
cp lectures/L02/exercises/atmega328p.cpp      libs/atmega/source/driver/gpio/atmega328p.cpp
cp lectures/L02/exercises/atmega328p_test.cpp libs/atmega/test/driver/gpio/atmega328p_test.cpp
```

**OBS!** Detta skriver över själva övningen. Vill ni ha tillbaka den ofärdiga testfilen och den
buggiga drivern, återställ dem med Git:

```bash
git restore libs/atmega/source/driver/gpio/atmega328p.cpp \
            libs/atmega/test/driver/gpio/atmega328p_test.cpp
```

Kör sedan testsviten från `libs/atmega/test`:

| Kommando | Beskrivning |
|---|---|
| `make` | Bygger och kör testsviten. |
| `make build` | Bygger enbart testsviten. |
| `make run` | Bygger vid behov och kör testsviten. |
| `make clean` | Tar bort testsviten samt `yrgo::test`-biblioteket. |

Samtliga 14 testfall ska passera - de fyra GPIO-testfallen, plus de tio som redan var gröna
(`adc`, `eeprom`, `tempsensor` samt `watchdog`):

```text
14 out of 14 test cases succeeded!
```

---

## Skriv tester
Testfilen innehåller fyra testfall, ett per GPIO-läge plus ett för initieringen:

| Testfall | Verifierar |
|---|---|
| `Initialization` | Att varje pin i intervallet [0, 19] går att reservera, att pinnar utanför intervallet nekas, samt att en andra instans på en redan reserverad pin *inte* initieras. |
| `Output` | Att `DDRx`-biten sätts, att `write()` och `toggle()` slår igenom i `PORTx`, samt att `DDRx` och `PORTx` nollställs när instansen försvinner. |
| `Input` | Att `DDRx`-biten är nollställd, att pull-upen är avstängd (`PORTx`-biten nollställd) och att `read()` returnerar det som ligger i `PINx`. |
| `InputPullup` | Samma sak som `Input`, men med pull-upen påslagen, dvs. `PORTx`-biten satt redan vid konstruktionen. |

Varje testfall loopar över samtliga pinnar per I/O-port, via en hjälpfunktion:

| Hjälpfunktion | Roll |
|---|---|
| `GpioRegs` | Samlar `DDRx`, `PORTx` och `PINx` för en I/O-port, så att samma testkod kan köras mot alla tre portarna. |
| `isPinValid(pin)` | Sant för pin 0 - 19, dvs. exakt de pinnar drivern ska acceptera. |
| `getRegBit(pin)` | Översätter pin-numret till bitnummer i portens register: pin 0 - 7 → `PORTD`-bit 0 - 7, pin 8 - 13 → `PORTB`-bit 0 - 5, pin 14 - 19 → `PORTC`-bit 0 - 5. |
| `simulateToggle(regs)` | Efterliknar hårdvarans toggling, se nedan. |
| `runOutputTest()`, `runInputTest()`, `runInputPullupTest()` | Kör ett helt scenario för en enskild pin. |

### Fyra val i testfilen värda att lägga märke till
* **GPIO-instansen skapas i ett eget scope `{ ... }`.** Då hinner destruktorn köra innan testet
  läser av `DDRx` och `PORTx` en sista gång, vilket är det enda sättet att verifiera att drivern
  städar upp efter sig. Det är också exakt där bugg 1 nedan exploderar.
* **`EXPECT_EQ(gpio.isInitialized(), pinValid)` i stället för `EXPECT_TRUE(...)`.** Samma rad
  täcker både giltiga och ogiltiga pinnar, vilket är det som gör att `Initialization` kan loopa
  ända upp till pin 49 utan att testkoden behöver dubbleras.
* **Varje `toggle()` följs av `simulateToggle()`.** På riktig hårdvara växlar en skrivning till
  `PINx` motsvarande bit i `PORTx`; den virtuella hårdvaruplattformen gör inte det åt oss (se
  [bilaga A](../appendix/a_gpio_testing.md)). Utan anropet hade `PORTx` aldrig ändrats, och man
  hade suttit och letat efter en driverbugg som inte finns.
* **Testerna itererar över samtliga pinnar, inte bara en per port.** Det kostar ingenting i
  körtid och är precis vad som krävs för att hitta bugg 2, som bara drabbar två av tjugo pinnar.

**Ett misslyckat `EXPECT_*` kastar ett undantag** och avbryter därmed hela testfallet. En loop
rapporterar alltså bara sin *första* trasiga pin, och två buggar i samma testfall syns en i taget.
Åtgärda en bugg, kör om, läs nästa felmeddelande.

---

## Buggjakt
Tre buggar finns inplanterade i [atmega328p.cpp](../../../libs/atmega/source/driver/gpio/atmega328p.cpp):

| Bugg | Var | Kort sagt |
|---|---|---|
| 1 | `~Atmega328p()` | Städar upp även när initieringen misslyckades, och avrefererar då en nollpekare. |
| 2 | `getIoPort()` | Täcker bara `Port::D0` - `Port::D5`, så pin 6 och 7 betraktas som ogiltiga. |
| 3 | `isPinFree()` | Kontrollerar bara pin-numret, inte pin-registret, så samma pin kan reserveras två gånger. |

### Så här ser första körningen ut
Med samtliga tre buggar kvar avslutas testsviten så här:

```text
Test case Adc_Atmega328p.Initialization succeeded!
Test case Adc_Atmega328p.Read succeeded!
Test case Eeprom_Atmega328p.Write succeeded!
Test case Eeprom_Atmega328p.Read succeeded!
Test case Gpio_Atmega328p.Initialization failed: EXPECT_FALSE(other.isInitialized()) failed: true != false (driver/gpio/atmega328p_test.cpp:236)
Segmentation fault (core dumped)
```

Notera att sammanfattningsraden (`N out of N test cases succeeded!`) aldrig skrivs ut: programmet
dör innan testsviten är klar. Det röda testfallet är bugg 3, kraschen är bugg 1 - som slår till
just på de pinnar bugg 2 gjort ogiltiga.

### Bugg 1 - destruktorn förutsätter att initieringen lyckades

```cpp
// Före: myHw är en nollpekare om konstruktorn inte kunde reservera hårdvaran.
enableInterrupt(false);
utils::clear(myHw->ddrx, myPin);
utils::clear(myHw->portx, myPin);
utils::clear(myPinRegistry, myId);
myHw = nullptr;

// Efter:
if (isInitialized())
{
    enableInterrupt(false);
    utils::clear(myHw->ddrx, myPin);
    utils::clear(myHw->portx, myPin);
    utils::clear(myPinRegistry, myId);
    myHw = nullptr;
}
```

Med enbart den här buggen kvar syns *inget* rött testfall alls - testsviten skriver ut de fyra
gröna testfallen före GPIO och dör sedan tyst, mitt i `Initialization`, när loopen når pin 20:
den första pin som aldrig kunde initieras. Precis det mönster [bilaga B](../appendix/b_exercises.md)
beskriver: ett objekt som inte kunde initieras försöker ändå städa upp efter sig.

Att ringa in kraschen: sänk `pinMax` i `Initialization` från 50 till 20, så att bara giltiga
pinnar testas. Försvinner kraschen då ligger felet i hanteringen av ogiltiga pinnar. Är alla tre
buggarna kvar räcker inte det, eftersom bugg 2 gör pin 6 och 7 ogiltiga mitt i `Output`-testets
loop över I/O-port D - minska då även den loopens övre gräns tills kraschen försvinner. Det
sista värdet ni tog bort är det som utlöser buggen.

I ett riktigt system hade den här buggen inte varit ett testfel utan en oförklarlig omstart, i
det ögonblick ett objekt på en felaktig pin gick ur scope.

### Bugg 2 - fel intervall för I/O-port D

```cpp
// Före:
else if (utils::inRange(id, Port::D0, Port::D5)) { return IoPort::D; }

// Efter:
else if (utils::inRange(id, Port::D0, Port::D7)) { return IoPort::D; }
```

I/O-port D har åtta pinnar (`PD0` - `PD7`), till skillnad från port B och C som har sex. Med
buggen kvar misslyckas samtliga fyra GPIO-testfall, alla på samma rad:

```text
Test case Gpio_Atmega328p.Initialization failed: EXPECT_EQ(gpio.isInitialized(), pinValid) failed: false != true (driver/gpio/atmega328p_test.cpp:231)
...
10 out of 14 test cases succeeded!
```

En GPIO på pin 6 eller 7 hade tyst slutat fungera: `write()` och `read()` gör ingenting på en
oinitierad GPIO, så lysdioden hade bara varit släckt. Det är därför `isInitialized()` kontrolleras
för *varje* pin i testet, i stället för att testet nöjer sig med en pin per port.

### Bugg 3 - pin-registret kontrolleras aldrig

```cpp
// Före:
constexpr bool isPinFree(const uint8_t id) noexcept { return PinCount > id; }

// Efter:
constexpr bool isPinFree(const uint8_t id) noexcept
{
    return PinCount > id ? !utils::read(myPinRegistry, id) : false;
}
```

Funktionen hette `isPinFree()`, men svarade i själva verket på frågan "är pin-numret giltigt?".
`initHw()` markerar pinnen som reserverad i `myPinRegistry`, men ingen läste registret. Symptomet
är ett enda rött testfall:

```text
Test case Gpio_Atmega328p.Initialization failed: EXPECT_FALSE(other.isInitialized()) failed: true != false (driver/gpio/atmega328p_test.cpp:236)
13 out of 14 test cases succeeded!
```

Två objekt hade då kunnat äga samma pin samtidigt: det ena skriver över det andras konfiguration,
och det som förstörs först nollställer `DDRx` och `PORTx` för dem båda.

---

## Vad som medvetet inte testas
* **Pin change-avbrotten** (`enableInterrupt()`, `enableInterruptOnPort()`) samt callbacken.
  I testbygget expanderar `ISR(vector)` till en helt vanlig funktion som ingen anropar, så själva
  avbrottet går inte att utlösa. `PCMSKx` går visserligen att läsa av, men `PCIE0`, `PCIE1` och
  `PCIE2` är i den virtuella hårdvaruplattformen mappade som minnesceller (alla noll) i stället
  för bitnummer, så en kontroll av `PCICR` hade inte kunnat skilja portarna åt.
* **`blink()`**, som anropar `utils::delay_ms()`. I testbygget blir det en riktig `sleep()`, så ett
  testfall som blinkar hade kostat exakt lika mycket tid som blinkningen. Ett långsamt test är ett
  test som slutar köras.

---
