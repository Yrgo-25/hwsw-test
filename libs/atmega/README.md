# ATmega328p-övningsbibliotek
Ett litet, färdigskrivet C++17-driverbibliotek för mikrokontrollern `ATmega328p` (samma familj
som Arduino Uno), som ni skriver enhets- och komponenttester för i **L02–L04** samt **L08**.
Biblioteket
används som en gemensam, delad övning innan ni tillämpar samma tekniker på er egen kodbas från
**P02** (`ESP32-S3`) i **P04**.

Biblioteket innehåller **medvetet inplanterade buggar**. Era tester är det som avslöjar dem.

---

## Varför ett annat chip än ESP32-S3?
`ATmega328p` är ett rent bare-metal-chip utan ett HAL-lager som ESP-IDF: drivers skriver
och läser hårdvaruregister direkt (t.ex. `DDRB`, `PORTB`), i stället för att anropa ett C-API.
Det gör det möjligt att demonstrera **virtuell hårdvara** i sin renaste form: i
[include/arch/test/hw_platform.h](./include/arch/test/hw_platform.h) mappas varje registernamn
om till en plats i ett vanligt minnesområde när koden byggs för värddatorn
(`-DTESTSUITE`), medan [include/arch/avr/hw_platform.h](./include/arch/avr/hw_platform.h)
inkluderar de riktiga AVR-headerfilerna vid byggnation för riktig hårdvara. Drivers egen
kod (t.ex. [source/driver/gpio/atmega328p.cpp](./source/driver/gpio/atmega328p.cpp)) är
identisk i båda fallen.

Er `ESP32-S3`-kodbas använder i stället ESP-IDF:s C-API, vilket kräver en annan teknik
(att mocka ESP-IDF:s C-funktioner, se **L06**). Principen, att låta produktionskoden vara
oförändrad medan testsviten länkas mot något annat än den riktiga hårdvaran, är densamma.

---

## Struktur
```text
include/   Interfaces, riktiga ATmega328p-drivers samt stubbar.
source/    Implementationer, inklusive den virtuella hårdvaruplattformen.
test/      Testsviten ni bygger vidare på, med tillhörande Makefile.
```

Biblioteket innehåller drivers för adc, eeprom, gpio, serial, tempsensor (`Tmp36`), timer och
watchdog, samt en `Logic`-klass som binder ihop dem. Samtliga följer samma mönster som **P02**:
ett interface, en riktig implementation, samt en hårdvaruoberoende stubb.

---

## Bygga och köra testsviten
```bash
cd libs/atmega/test
make          # Bygger yrgo::test, biblioteket samt testsviten, och kör den.
make build    # Bygger utan att köra.
make run      # Kör utan att bygga om.
make clean    # Städar bort byggda filer, både testsviten och yrgo::test.
```

Testerna för `adc`, `eeprom`, `tempsensor::Tmp36` samt `watchdog` är redan kompletta och ska alla
vara gröna direkt (10 testfall). Testerna för `gpio` (**L02**), `serial` (**L03**), `timer`
(**L04**) samt stubbarna för `gpio`, `timer` och `tempsensor` (**L08**) är avsiktligt
inaktiverade tills vidare.

---

## Lektionsuppdelade övningar
Varje ofullständig testfil är omsluten av ett `#ifdef`-block, t.ex.:

```cpp
//! @todo Remove this #ifdef in lecture 2 to enable these tests.
#ifdef LECTURE2
// ... testfall att fylla i, med instruktioner i kommentarer ...
#endif /** LECTURE2 */
```

Ta bort `#ifdef`- och `#endif`-raderna (inte innehållet mellan dem) för att aktivera respektive
testfil, och fyll sedan i testfallen enligt kommentarerna i filen:
* [driver/gpio/atmega328p_test.cpp](./test/driver/gpio/atmega328p_test.cpp) (`LECTURE2`, **L02**)
* [driver/serial/atmega328p_test.cpp](./test/driver/serial/atmega328p_test.cpp) (`LECTURE3`, **L03**)
* [driver/timer/atmega328p_test.cpp](./test/driver/timer/atmega328p_test.cpp) (`LECTURE4`, **L04**)

Kör om testsviten efter varje ifyllt testfall. Om ett test misslyckas: avgör om felet ligger i
ert test eller i drivern, och åtgärda i så fall buggen i motsvarande fil under
[source/driver](./source/driver).

**L08** är av en annan sort: `driver::gpio::Stub`, `driver::timer::Stub` samt
`driver::tempsensor::Stub` är fortfarande tomma (se
[include/driver/gpio/stub.h](./include/driver/gpio/stub.h),
[include/driver/timer/stub.h](./include/driver/timer/stub.h) samt
[include/driver/tempsensor/stub.h](./include/driver/tempsensor/stub.h)), till skillnad från
övriga drivers stubbar som redan är kompletta. Ni skrev motsvarande stubbar i en tidigare kurs,
så det här är repetition. Här är det själva stubben, inte testfilen, som ska skrivas:
* [driver/gpio/stub_test.cpp](./test/driver/gpio/stub_test.cpp) (`LECTURE8`, **L08**)
* [driver/timer/stub_test.cpp](./test/driver/timer/stub_test.cpp) (`LECTURE8`, **L08**)

`driver::tempsensor::Stub` har ingen egen testfil; den behövs för komponenttestet nedan och ska
kunna ge ett förbestämt temperaturvärde.

Ta bort `#ifdef LECTURE8`/`#endif` i båda testfilerna, implementera stubbarna mot respektive
`Interface` (se **P02**s egna stubbar för samma mönster), och kör om testsviten tills testerna
är gröna.

**Två krav som är lätta att missa:**
* **Ge samtliga konstruktorparametrar default-värden**, som i
  [include/driver/adc/stub.h](./include/driver/adc/stub.h). Testfilerna skapar stubbarna med
  argument (`gpio::Stub{Mode::Input}`, `timer::Stub{100U}`), medan
  [logic/logic_test.cpp](./test/logic/logic_test.cpp) skapar dem helt utan argument. En stubb med
  bara `explicit Stub(Mode)` klarar stubbtesterna men går inte att bygga ihop med
  komponenttestet.
* **`isInterruptEnabled()` i `driver::gpio::Stub` ska spegla både pin-masken och portens
  aktivering**, precis som `enableInterrupt()`/`enableInterruptOnPort()` i
  [source/driver/gpio/atmega328p.cpp](./source/driver/gpio/atmega328p.cpp). Två oberoende
  booleaner räcker inte: det färdiga testfallet `DebounceHandling` förutsätter att
  `enableInterruptOnPort(false)` slår igenom på pin-nivå.

Biblioteket innehåller även en färdig `logic::Logic`, som binder ihop samtliga sju drivers,
med ett tillhörande komponenttest i [logic/logic_test.cpp](./test/logic/logic_test.cpp). Det
testet är omslutet av `#ifdef STUBS_IMPLEMENTED` i stället för en enskild `LECTUREn`, eftersom
det förutsätter att samtliga tre stubbar ovan precis skrivits färdigt. Ett
av testfallen (`DebounceHandling`) är komplett, medan övriga (`ToggleHandling`, `TempHandling`,
`Eeprom`) ska fyllas i enligt kommentarerna i filen, precis som **L02–L04**s testfiler. Allt
detta sker under **L08**, som en uppvärmning inför komponenttester av er egen systemlogik och
stubbar.

---
