# L09 - Komponenttester (del II)

## Dagordning
* Repetition: skriv `driver::gpio::Stub` samt `driver::timer::Stub` i det delade
  ATmega328p-övningsbiblioteket.
* Varför en stubbs utgångsläge och beteende måste motsvara den riktiga drivrutinens.
* Genomgång i helklass.

---

## Mål med lektionen
* Kunna skriva en stubb mot ett givet interface (repetition från en tidigare kurs).
* Ha fungerande `driver::gpio::Stub` och `driver::timer::Stub`, med gröna stubbtester, redo att
  användas i komponenttesterna i **L11**.

---

## Instruktioner

### Innan lektionen
* Repetera [bilaga A i L08](../L08/appendix/a_component_tests.md) om komponenttester.

### Under lektionen
* Genomför övningsuppgifterna i [bilaga A](./appendix/a_exercises.md):
  * Ni får tid på er att genomföra uppgiften enskilt eller i grupp.
  * Uppgiften gås sedan igenom i helklass.

---

## Utvärdering
* Vilka val gjorde ni för `driver::gpio::Stub` respektive `driver::timer::Stub`, t.ex. vilka
  hjälpmetoder ni lade till utöver interfacet?
* Varför måste en stubbs utgångsläge motsvara den riktiga drivrutinens, för att ett
  komponenttest ska säga något om `Logic`?

---

## Nästa lektion
* Praktiskt prov 1 (gemensamt med Maskininlärning).

---
