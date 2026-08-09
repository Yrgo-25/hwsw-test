# L08 - Komponenttester (del I)

## Dagordning
* Repetition: skriv `driver::gpio::Stub` samt `driver::timer::Stub` i det delade
  ATmega328p-övningsbiblioteket.
* Vad är ett komponenttest, och hur skiljer det sig från ett enhetstest?
* Testning av `system::logic::Logic` med samtliga sex drivers stubbade.
* Fortsatt arbete med **P04**.

---

## Mål med lektionen
* Kunna skriva en stubb mot ett givet interface (repetition från en tidigare kurs).
* Kunna förklara skillnaden mellan ett enhetstest och ett komponenttest.
* Kunna sätta ihop `Logic` med en `driver::factory::Stub` och komma åt de skapade
  stubb-instanserna i ett test.
* Kunna verifiera samspelet mellan `Logic` och dess drivers, t.ex. att kommandot `"on"`
  resulterar i att `driver::gpio::Stub` sätts till rätt nivå.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_component_tests.md) om komponenttester.

### Under lektionen
* Delta aktivt på genomgången.
* Bygg upp `Logic` i testet med en `driver::factory::Stub` i stället för
  `driver::factory::Esp32s3`.
* Genomför övningsuppgifterna i [bilaga B](./appendix/b_exercises.md):
  * Ni får tid på er att genomföra uppgiften enskilt eller i grupp.
  * Uppgiften gås sedan igenom i helklass.

---

## Utvärdering
* Vilka val gjorde ni för `driver::gpio::Stub` respektive `driver::timer::Stub`, t.ex. vilka
  hjälpmetoder ni lade till utöver interfacet?
* Vad testar ett komponenttest som ett enhetstest inte fångar, och tvärtom?
* Hur exponerar er `driver::factory::Stub` de stubb-instanser den skapar, så att testet kan
  styra och läsa av dem?
* Varför använder ni stubbar för samtliga drivers i stället för de riktiga klasserna när ni
  komponenttestar `Logic`?

---

## Nästa lektion
* Komponenttester (del II).
* Fortsatt arbete med **P04**.

---
