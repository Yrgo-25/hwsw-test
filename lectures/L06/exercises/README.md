# L06 - Lektionsanteckningar, mock-exempel
Ett litet, komplett exempel på tekniken i [bilaga B](../appendix/b_stubs_and_virtual_hw.md): en
GPIO-driver för ESP32 som anropar ESP-IDF:s C-funktioner, byggd och körd på värddatorn mot en
mock i stället för mot riktig hårdvara.

Det här är **inget lösningsförslag** till [bilaga C](../appendix/c_exercises.md). Den övningen
görs på er egen `driver::gpio::Esp32s3`, i ert eget repo. Exemplet visar mekaniken i minsta
möjliga format, så att ni har något att jämföra med när ni bygger motsvarande i er kodbas.

---

## Innehåll

```text
exercises/
├── include/
│   ├── arch/env/
│   │   ├── hw_platform.h               - Väljer mocken eller ESP-IDF, beroende på TESTSUITE.
│   │   └── test/hw_platform.h          - Den mockade plattformen, inkluderar mock-headern.
│   └── driver/gpio/esp32.h             - driver::gpio::Esp32, en minimal GPIO-driver.
├── source/
│   └── driver/gpio/esp32.cpp           - Drivern, som anropar ESP-IDF:s C-funktioner.
└── test/
    ├── Makefile                        - Bygger och kör testprogrammet på värddatorn.
    ├── include/test/esp32/gpio_mock.h  - Mockens deklarationer.
    ├── source/test/esp32/gpio_mock.c   - Mockens implementation.
    └── testsuite.cpp                   - Testprogrammet.
```

`include/` och `source/` motsvarar er firmware: det är den koden som hade byggts för hårdvaran.
Allt under `test/` finns bara för att kunna köra samma kod på värddatorn.

---

## Bygga och köra
Exemplet använder inget testramverk, så ingen submodul behövs. Kör från `test/`:

| Kommando | Beskrivning |
|---|---|
| `make` | Bygger och kör testprogrammet. |
| `make build` | Bygger enbart testprogrammet. |
| `make run` | Kör testprogrammet. |
| `make clean` | Tar bort testprogrammet. |

```text
$ make
GPIO state: off
GPIO state: on
```

Makefilen bygger allt med `g++` och flaggan `-DTESTSUITE`, och länkar in `gpio_mock.c` tillsammans
med drivern. Notera att det gör att även `gpio_mock.c` kompileras som C++, trots filändelsen.

---

## Hur valet mellan mock och ESP-IDF görs
Drivern inkluderar aldrig ESP-IDF:s `driver/gpio.h` direkt, utan en egen, projektägd header som
gör valet på ett enda ställe:

```text
esp32.cpp
  └── #include "arch/env/hw_platform.h"
        ├── utan TESTSUITE:   #include "driver/gpio.h"            ESP-IDF, riktig hårdvara
        └── med -DTESTSUITE:  #include "arch/env/test/hw_platform.h"
                                └── #include "test/esp32/gpio_mock.h"   mocken, värddatorn
```

Drivern själv innehåller alltså ingen `#ifdef`, och anropar exakt samma funktioner i båda
byggena. Det är samma idé som `arch/avr/hw_platform.h` respektive `arch/test/hw_platform.h` i
[ATmega328p-övningsbiblioteket](../../../libs/atmega/README.md), som ni använde i **L02-L04**.

Det finns två sätt att åstadkomma samma sak, och bilaga B beskriver det andra i sitt tips längst
ned: att döpa mock-headern till `driver/gpio.h` och lägga dess katalog först i kompilatorns
sökväg. Då kan drivern inkludera `driver/gpio.h` precis som vanligt. Exemplet väljer i stället en
egen `hw_platform.h`, vilket kostar en extra inkluderingsnivå men gör valet synligt i koden i
stället för gömt i byggsystemets `-I`-flaggor.

---

## Drivern
`driver::gpio::Esp32` är avsiktligt så liten som möjligt, med tre anrop till ESP-IDF:

| Metod | Anropar | Gör |
|---|---|---|
| Konstruktorn | `gpio_config()` | Konfigurerar pinnen. Samtliga fält utom `pin_bit_mask` sätts till 0. |
| `write(state)` | `gpio_set_level()` | Sätter pinnens nivå. |
| `read()` | `gpio_get_level()` | Läser pinnens nivå. |

I en riktig driver hade läge, pull-up/pull-down samt avbrottstyp satts utifrån pinnens
datariktning, som kommentaren i konstruktorn påpekar. Er egen `driver::gpio::Esp32s3` är alltså
betydligt mer omfattande, och anropar sannolikt fler ESP-IDF-funktioner än de här tre.

---

## Mocken
Mocken implementerar samma funktioner som ESP-IDF, med samma namn och parametrar, inom
`extern "C"` så att C++-koden länkar mot dem med C-länkning, precis som mot originalen:

| Funktion | I ESP-IDF | I mocken |
|---|---|---|
| `gpio_config()` | Konfigurerar en eller flera pinnar | Sparar en *kopia* av konfigurationen. |
| `gpio_set_level()` | Sätter en utnivå | Sätter eller rensar pinnens bit i `pin_reg`. En pin över 48 ger `ESP_ERR_INVALID_ARG`. |
| `gpio_get_level()` | Läser en nivå | Läser pinnens bit ur `pin_reg`. Pinnen valideras inte. |
| `gpio_last_config()` | Finns inte | Returnerar den senast sparade konfigurationen. |

Tre saker är värda att lägga märke till:
* **`gpio_last_config()` är en spion.** Den finns inte i ESP-IDF, utan är tillagd enbart för att
  ett test ska kunna verifiera *hur* `gpio_config()` anropades. `gpio_set_level()` behöver ingen
  motsvarighet, eftersom `gpio_get_level()` redan läser tillbaka det som sattes.
* **Mocken förenklar typerna.** `gpio_num_t` är här en `uint8_t` och konfigurationsfälten är
  `int`, medan ESP-IDF använder uppräkningstyper för dem. Det räcker för det här exemplet, men
  det är precis här mock och verklighet kan glida isär: som bilaga B påpekar matchar länkaren
  bara funktionsnamnen, inte typerna.
* **Mocken har globalt tillstånd.** `pin_reg` och `last_config` är filglobala och lever lika
  länge som testprogrammet. Testfall som körs efter varandra i samma program delar därför på
  dem, och ett test får inte förutsätta att en pin är låg bara för att det själv inte satt den.
  Det är just den fallgrop bilaga A varnar för under *Globalt eller statiskt delat tillstånd*.

---

## Testprogrammet
`testsuite.cpp` skapar en GPIO på pin 9, läser av den, sätter den hög och läser av den igen:

```cpp
constexpr std::uint8_t led1Pin{9U};
driver::gpio::Esp32 led1{9U};

const auto state1 = led1.read();
led1.write(true);
const auto state2 = led1.read();
```

Programmet visar att drivern går att köra på värddatorn, men trots namnet är det **ännu ingen
testsvit**: det skriver bara ut resultatet, verifierar ingenting och returnerar alltid 0. Ett
CI-jobb som körde programmet hade alltså varit grönt även om drivern skrivit till fel pin.
`gpio_last_config()` används inte heller än.

Att göra om det till riktiga enhetstester, med `yrgo::test` och `EXPECT_*` mot både
`gpio_get_level()` och `gpio_last_config()`, är precis vad bilaga C ber er göra för er egen
`driver::gpio::Esp32s3`. Då behöver makefilen dessutom bygga och länka mot testramverket, på
samma sätt som i **L01** och i övningsbiblioteket.

---

## Vad som medvetet inte ingår
* **Enhetstester**, se ovan.
* **Något bygge mot riktig ESP-IDF.** Grenen utan `TESTSUITE` i `hw_platform.h` inkluderar
  ESP-IDF:s `driver/gpio.h`, som bara finns i ett ESP-IDF-bygge. Exemplet byggs enbart för
  värddatorn.
* **Felhantering i drivern.** Mockens `gpio_set_level()` returnerar `ESP_ERR_INVALID_ARG` för en
  ogiltig pin, men drivern ignorerar returvärdet. Ett test som vill verifiera det beteendet måste
  därför gå direkt på mocken, inte via drivern.

---
