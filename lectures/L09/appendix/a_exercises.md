# Bilaga A - Övningsuppgifter

## Skriv `driver::gpio::Stub` och `driver::timer::Stub`
I det delade [ATmega328p-övningsbiblioteket](../../../libs/atmega/README.md) är
`driver::gpio::Stub` samt `driver::timer::Stub` fortfarande tomma. Ni skrev
`driver::tempsensor::Stub` i **L08**; nu gör ni samma sak för de två återstående:
* Implementera `driver::gpio::Stub` mot `driver::gpio::Interface` samt `driver::timer::Stub` mot
  `driver::timer::Interface`. Använd bibliotekets övriga stubbar (t.ex.
  [driver/adc/stub.h](../../../libs/atmega/include/driver/adc/stub.h)) som mall.
* Ta bort `#ifdef LECTURE8`/`#endif` i
  [driver/gpio/stub_test.cpp](../../../libs/atmega/test/driver/gpio/stub_test.cpp) samt
  [driver/timer/stub_test.cpp](../../../libs/atmega/test/driver/timer/stub_test.cpp), och kör
  testsviten tills stubbtesterna är gröna:

  ```bash
  cd libs/atmega/test
  make
  ```

Om ni tar bort `#ifdef LECTURE8` *innan* stubbarna finns får ni en lång rad kompileringsfel
(`'gpio' has not been declared` m.fl.). Det är väntat, och försvinner när stubbarna är på plats.

---

## Krav som är lätta att missa
Stubbarna används i **L11** av komponenttestet
[logic/logic_test.cpp](../../../libs/atmega/test/logic/logic_test.cpp). Det ställer några krav
utöver att stubbtesterna blir gröna:
* **Ge samtliga konstruktorparametrar default-värden**, som i `adc/stub.h`. Stubbtesterna skapar
  stubbarna med argument (`gpio::Stub{Mode::Input}`, `timer::Stub{100U}`), medan
  `logic_test.cpp` skapar dem helt utan argument.
* **Utgångsläget ska motsvara den riktiga drivern.** Den riktiga `driver::timer::Atmega328p`
  startar inte timern om det inte uttryckligen begärs, och det ska inte stubben heller. En
  timer-stubb som startar sig själv i konstruktorn gör att `Logic` senare *växlar av* en timer
  som testet förväntar sig ska bli påslagen, och felet ser då ut att ligga i `Logic`.
* **Pin och port är två olika saker.** `enableInterrupt()` styr pinnens egen avbrottsmask,
  `enableInterruptOnPort()` hela portens, precis som `PCMSK`/`PCICR` i den riktiga drivern. Med
  en enda gemensam flagga skriver de över varandra. `isInterruptEnabled()` ska bara vara sann när
  *båda* är aktiverade, och stubbtestet förutsätter dessutom en `isPortInterruptEnabled()`.

---

## Diskussion
* Vilka hjälpmetoder lade ni till utöver interfacet, och varför behövs de i ett test?
* Varför är det värt att ha egna tester för stubbarna, när de ändå bara är testverktyg?

---
