# Bilaga B - Övningsuppgifter

## Repetition: skriv `driver::tempsensor::Stub`
I det delade [ATmega328p-övningsbiblioteket](../../../libs/atmega/README.md) är tre stubbar
fortfarande tomma, till skillnad från bibliotekets övriga: `driver::tempsensor::Stub`,
`driver::gpio::Stub` samt `driver::timer::Stub`. Ni skrev motsvarande stubbar i en tidigare kurs,
så det här är repetition. Idag skriver ni den första; `gpio` och `timer` följer i **L09**.

Implementera `driver::tempsensor::Stub` mot `driver::tempsensor::Interface` i
[include/driver/tempsensor/stub.h](../../../libs/atmega/include/driver/tempsensor/stub.h). Använd
bibliotekets övriga stubbar (t.ex.
[driver/adc/stub.h](../../../libs/atmega/include/driver/adc/stub.h)) som mall. Stubben ska:
* Implementera interfacets metoder, `isInitialized()` samt `read()`.
* Ha en temperatur som testet själv kan styra, både via konstruktorn och via en hjälpmetod, så att
  ett komponenttest kan ge `Logic` en förbestämd temperatur och verifiera vad som skrivs ut.
* Gå att simulera som oinitierad via en hjälpmetod, så att ett test kan verifiera hur `Logic`
  hanterar en sensor som inte fungerar.
* Ha ett default-värde för samtliga konstruktorparametrar, som i `adc/stub.h`. Komponenttestet
  [logic/logic_test.cpp](../../../libs/atmega/test/logic/logic_test.cpp), som ni fyller i under
  **L11**, skapar stubbarna helt utan argument.

Stubben har ingen egen testfil. Kontrollera att testsviten fortfarande går att bygga och att
samtliga befintliga tester är gröna:

```bash
cd libs/atmega/test
make
```

---

## Diskussion
* Vad testar ett komponenttest som ett enhetstest inte fångar, och tvärtom?
* Vad skulle hända med testets hastighet och determinism om `Logic` använde den riktiga
  temperatursensorn i stället för en stubb i ett komponenttest?

---
