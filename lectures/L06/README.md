# L06 - Testbar design, stubbar och virtuell hårdvara

## Dagordning
* Testbar design: interfaces som gräns mellan logik och hårdvara, och dependency injection direkt
  via konstruktorn respektive via en factory (som i **P02**).
* Skillnaden mellan en stubb, en mock och en fake.
* Repetition: era befintliga stubbar från **P02**, och vilken nivå de testar (**L02–L04**).
* Virtuell hårdvara: att mocka ESP-IDF:s C-funktioner för att enhetstesta driverklassernas egen
  logik på värddatorn.
* Fortsatt arbete med **P04**.

---

## Mål med lektionen
* Kunna förklara varför testbarhet är en designegenskap, inte något som tillkommer i efterhand.
* Kunna förklara hur `driver::factory::Interface` gör det möjligt att växla mellan
  `system::logic::Logic` i hårdvaruläge och simulerat läge utan att ändra `Logic` själv.
* Kunna resonera kring vad som hade behövt ändras om en klass i stället skapade sina beroenden
  internt.
* Kunna förklara skillnaden mellan stubbar, mockar och fakes, samt när respektive lämpar sig.
* Kunna förklara vilken nivå era befintliga `driver::*::Stub`-klasser testar, jämfört med de
  riktiga `Esp32s3`-klasserna.
* Kunna testa hårdvarunära kod på värddatorn genom att mocka de ESP-IDF-funktioner den anropar.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_testable_design.md) om testbar design.
* Läs [bilaga B](./appendix/b_stubs_and_virtual_hw.md) om stubbar och virtuell hårdvara.

### Under lektionen
* Delta aktivt på genomgången.
* Gå igenom hur `system::logic::Logic` respektive `driver::tempsensor::Tmp36` tar emot sina
  beroenden i **P02**, och varför det gör dem testbara.
* Genomför övningsuppgifterna i [bilaga C](./appendix/c_exercises.md) i grupp.

---

## Utvärdering
* Vad är skillnaden mellan en klass som tar emot sina beroenden via konstruktorn (via referenser
  eller via en factory), och en klass som skapar dem internt, sett till testbarhet?
* Varför tar er `Logic` emot en hel factory i stället för sina drivers en och en?
* Vad är skillnaden mellan en stubb och en mock, sett till hur testresultatet verifieras?
* Era stubbar från **P02** (t.ex. `driver::gpio::Stub`) och en mockad ESP-IDF-funktion löser
  liknande problem, hårdvaruoberoende. Vad är skillnaden i vad de faktiskt testar?
* Varför behöver `driver::gpio::Esp32s3`s egen kod inte ändras alls för att kunna testas mot en
  mockad ESP-IDF-funktion, och vad är det som styr om testsviten länkar mot mocken eller mot
  ESP-IDF:s riktiga komponent?

---

## Nästa lektion
* Enhetstester (del IV).
* Fortsatt arbete med **P04**.

---
