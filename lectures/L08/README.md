# L08 - Komponenttester (del I)

## Dagordning
* Vad är ett komponenttest, och hur skiljer det sig från ett enhetstest?
* Repetition: skriv `driver::tempsensor::Stub` i det delade ATmega328p-övningsbiblioteket.
* Fortsatt arbete med **P04**.

---

## Mål med lektionen
* Kunna förklara skillnaden mellan ett enhetstest och ett komponenttest.
* Kunna skriva en stubb mot ett givet interface (repetition från en tidigare kurs).
* Kunna förklara varför en stubb behöver ett värde som testet själv kan styra, t.ex. en
  förbestämd temperatur, för att kunna användas i ett komponenttest.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_component_tests.md) om komponenttester.

### Under lektionen
* Delta aktivt på genomgången.
* Genomför övningsuppgiften i [bilaga B](./appendix/b_exercises.md):
  * Ni får tid på er att genomföra uppgiften enskilt eller i grupp.
  * Uppgiften gås sedan igenom i helklass.
* Lösningsförslag finns [här](./exercises/README.md).
* Resterande tid: fortsatt arbete med enhetstesterna i **P04**.

---

## Utvärdering
* Vilka hjälpmetoder lade ni till i `driver::tempsensor::Stub` utöver interfacet, och varför?
* Vad testar ett komponenttest som ett enhetstest inte fångar, och tvärtom?
* Varför använder man stubbar för samtliga drivers i stället för de riktiga klasserna när man
  komponenttestar `Logic`?

---

## Nästa lektion
* Komponenttester (del II): `driver::gpio::Stub` samt `driver::timer::Stub`.

---
