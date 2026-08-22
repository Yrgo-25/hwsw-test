# Bilaga B - Övningsuppgifter

## Repetition: skriv bibliotekets tre tomma stubbar
I det delade [ATmega328p-övningsbiblioteket](../../../libs/atmega/README.md) är
`driver::gpio::Stub`, `driver::timer::Stub` samt `driver::tempsensor::Stub` fortfarande tomma,
till skillnad från bibliotekets övriga stubbar. Ni skrev motsvarande stubbar i en tidigare kurs,
så det här är repetition:
* Implementera `driver::gpio::Stub` mot `driver::gpio::Interface`, `driver::timer::Stub` mot
  `driver::timer::Interface` samt `driver::tempsensor::Stub` mot
  `driver::tempsensor::Interface`. Använd bibliotekets övriga stubbar (t.ex.
  [driver/adc/stub.h](../../../libs/atmega/include/driver/adc/stub.h)) som mall.
* Ta bort `#ifdef LECTURE8`/`#endif` i
  [driver/gpio/stub_test.cpp](../../../libs/atmega/test/driver/gpio/stub_test.cpp) samt
  [driver/timer/stub_test.cpp](../../../libs/atmega/test/driver/timer/stub_test.cpp).
  `driver::tempsensor::Stub` har ingen egen testfil, men behövs för komponenttestet nedan och
  ska kunna ge ett förbestämt temperaturvärde.
* Bygg och kör testsviten:

  ```bash
  cd libs/atmega/test
  make
  ```

**Två krav som är lätta att missa:**
* **Ge samtliga konstruktorparametrar default-värden**, som i `adc/stub.h`. Stubbtesterna skapar
  stubbarna med argument (`gpio::Stub{Mode::Input}`, `timer::Stub{100U}`), medan
  `logic_test.cpp` nedan skapar dem helt utan argument. En stubb med bara
  `explicit Stub(Mode)` klarar stubbtesterna men går inte att bygga ihop med komponenttestet.
* **`isInterruptEnabled()` i `driver::gpio::Stub` ska spegla både pin-masken och portens
  aktivering**, precis som den riktiga drivern gör med `PCMSK`/`PCICR`. Två oberoende booleaner
  räcker inte: det färdiga testfallet `DebounceHandling` förutsätter att
  `enableInterruptOnPort(false)` slår igenom på pin-nivå.

Om ni tar bort `#ifdef LECTURE8` *innan* stubbarna finns får ni en lång rad kompileringsfel
(`'gpio' has not been declared` m.fl.). Det är väntat, och försvinner när stubbarna är på plats.

---

## Uppvärmning: färdigställ `logic_test.cpp`
Ta bort `#ifdef STUBS_IMPLEMENTED`/`#endif` i det delade övningsbibliotekets
[logic/logic_test.cpp](../../../libs/atmega/test/logic/logic_test.cpp), som går att bygga först
när **alla tre** stubbar ovan är färdiga, och fyll i de återstående testfallen (`ToggleHandling`,
`TempHandling`, `Eeprom`) enligt kommentarerna i filen. Kör om testsviten tills alla tester blir
gröna.

---

## Skriv ett komponenttest för er egen `Logic`
Utgå från exemplet i bilaga A och skriv ett komponenttest som verifierar `"on"`-kommandot. Bygg
därefter ut testsviten med minst ett av de övriga scenarierna (blinkläge, temperatur, status).

---

## Diskussion
* Vad testar ett komponenttest som ett enhetstest inte fångar, och tvärtom?
* Hur löste ni problemet med att komma åt de stubb-instanser som `driver::factory::Stub` skapar
  internt åt `Logic`?
* Vad skulle hända med testets hastighet och determinism om ni använde de riktiga
  `Esp32s3`-drivers i stället för stubbar i ett komponenttest?

---
