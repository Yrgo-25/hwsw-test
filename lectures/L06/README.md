# L06 - Stubbar och virtuell hårdvara

## Dagordning
* Skillnaden mellan en stubb, en mock och en fake.
* Repetition: era befintliga stubbar från **P02**, och vilken nivå de testar (**L02–L04**).
* Virtuell hårdvara: att mocka ESP-IDF:s C-funktioner för att enhetstesta driverklassernas egen
  logik på värddatorn.
* Fortsatt arbete med **P04**.

---

## Mål med lektionen
* Kunna förklara skillnaden mellan stubbar, mockar och fakes, samt när respektive lämpar sig.
* Kunna förklara vilken nivå era befintliga `driver::*::Stub`-klasser testar, jämfört med de
  riktiga `Esp32s3`-klasserna.
* Kunna testa hårdvarunära kod på värddatorn genom att mocka de ESP-IDF-funktioner den anropar.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_stubs_and_virtual_hw.md) om stubbar och virtuell hårdvara.

### Under lektionen
* Delta aktivt på genomgången.
* Genomför övningsuppgifterna i [bilaga B](./appendix/b_exercises.md) i grupp.

---

## Utvärdering
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
