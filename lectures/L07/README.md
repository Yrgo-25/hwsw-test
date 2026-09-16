# L07 - Referensvärden och toleranser

## Dagordning
* Enhetstestning av en temperaturformel: flyttalsjämförelser, `EXPECT_NEAR` och val av
  tolerans.
* Fortsatt arbete med **P04**.

---

## Mål med lektionen
* Kunna välja en rimlig tolerans för `EXPECT_NEAR` vid flyttalsjämförelser.
* Kunna tillämpa mockningstekniken från **L06** självständigt på fler av era riktiga
  `Esp32s3`-drivers.
* Kunna verifiera en ML-beräkning (er `ml::lin_reg::Adaptive`) mot ett framräknat referensvärde.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_testing_tmp36.md) om enhetstestning av `Tmp36`.

### Under lektionen
* Delta aktivt på genomgången.
* Genomför övningsuppgifterna i [bilaga B](./appendix/b_exercises.md).
* Lösningsförslag finns [här](./exercises/README.md).
* Resterande tid: skriv, på egen hand i gruppen, enhetstester för fler av era riktiga
  `Esp32s3`-drivers (t.ex. `adc`, `gpio`, `timer`) med mockningstekniken från **L06**, samt
  enhetstester för era ML-algoritmer från **P03** (`ml::lin_reg::Adaptive`).

---

## Utvärdering
* Hur avgjorde ni vilken tolerans (`EXPECT_NEAR`) som var rimlig när ni jämförde flyttal?
* Två matematiskt identiska uttryck gav ändå olika resultat i övningen. Vad avgör om
  `EXPECT_EQ` eller `EXPECT_NEAR` är rätt makro?

---

## Nästa lektion
* Komponenttester (del I).
* Fortsatt arbete med **P04**.

---
