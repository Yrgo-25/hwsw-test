# L07 - Enhetstester (del IV)

## Dagordning
* Enhetstestning av `driver::tempsensor::Tmp36`s temperaturformel från **P02**.
* Testning av beräkningar mot kända, framräknade referensvärden.
* Samma teknik tillämpad på era ML-algoritmer från **P03**, ett **G**-krav i **P04**.
* Resten av lektionen: enhetstester för fler av era riktiga `Esp32s3`-drivers, på egen hand i
  gruppen.
* Fortsatt arbete med **P04**.

---

## Mål med lektionen
* Kunna verifiera en beräkning mot ett känt, för hand uträknat exempel.
* Kunna välja en rimlig tolerans för `EXPECT_NEAR` vid flyttalsjämförelser.
* Förstå varför `Tmp36` är testbar redan nu, trots att den ligger i driverlagret.
* Kunna tillämpa mockningstekniken från **L06** självständigt på fler av era riktiga
  `Esp32s3`-drivers.
* Kunna verifiera en ML-beräkning (`ml::lin_reg::Adaptive`) mot ett framräknat referensvärde.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_testing_tmp36.md) om enhetstestning av `Tmp36`.

### Under lektionen
* Delta aktivt på genomgången.
* Genomför övningsuppgifterna i [bilaga B](./appendix/b_exercises.md):
  * Ni får tid på er att genomföra uppgiften enskilt.
  * Uppgiften gås sedan igenom i helklass.
* Resterande tid: skriv, på egen hand i gruppen, enhetstester för fler av era riktiga
  `Esp32s3`-drivers (t.ex. `adc`, eller `eeprom`/`watchdog` om er grupp implementerade dem i
  **P02**) med mockningstekniken från **L06**, samt enhetstester för era ML-algoritmer från
  **P03** (`ml::lin_reg::Adaptive` samt minst ett neuralt nätverkslager).

---

## Utvärdering
* Hur avgjorde ni vilken tolerans (`EXPECT_NEAR`) som var rimlig när ni jämförde flyttal?
* Varför behöver `Tmp36` ingen ESP-IDF-mockning för att enhetstestas, till skillnad från
  `driver::adc::Esp32s3`?

---

## Nästa lektion
* Komponenttester (del I).
* Fortsatt arbete med **P04**.

---
