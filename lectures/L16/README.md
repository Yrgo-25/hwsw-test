# L16 - Mätdatainsamling (del I): insamling och lagring

## Dagordning
* Introduktion till kursens avslutande del, som hålls av en gästlärare: mätdatainsamling och
  testautomatisering med Python mot fysisk hårdvara.
* Genomgång: insamling av mätdata från en DUT via seriell kommunikation, samt strukturerad
  lagring.
* Arbete med mätdatadelen av **P04**.

---

## Mål med lektionen
* Kunna definiera och dokumentera ett enkelt protokoll för mätdata över seriell kommunikation.
* Kunna läsa mätdata från `ESP32-S3` med ett Python-skript och `pyserial`, och hantera trasiga
  eller ofullständiga rader på ett genomtänkt sätt.
* Kunna lagra mätdata strukturerat som CSV, så att den går att bearbeta och upprepa i efterhand.
* Ha en första mätning från ert eget system incheckad i ert repo.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_measurement_data.md) om insamling och lagring av mätdata.
* Ta med ert `ESP32-S3`-kort och en USB-kabel, och kontrollera att er firmware går att flasha
  (se **L15**).
* Installera kursens Python-beroenden (se [Kodformattering](../../README.md#kodformattering) i
  kursrepots `README.md`).

### Under lektionen
* Delta aktivt på genomgången.
* Genomför därefter uppgifterna i [bilaga B](./appendix/b_exercises.md), i grupp och i ert eget
  repo. De gås inte igenom i helklass, eftersom varje grupps protokoll och firmware ser olika ut;
  läraren går runt och hjälper till under tiden.

---

## Utvärdering
* Varför är det viktigt att ha ett väldefinierat, dokumenterat format på datan som skickas över
  seriell kommunikation?
* Hur hanterar ert skript en trasig rad, och varför valde ni just det sättet?

---

## Nästa lektion
* Mätdatainsamling (del II): processning och visualisering jämfört mot kravställning.

---
