# L01 - Kursintroduktion, teststrategi och testpyramiden

## Dagordning
* Introduktion: genomgång av kursens innehåll samt upplägg.
* Varför testar vi mjukvara? Kostnaden för buggar i olika utvecklingsskeden.
* Testpyramiden: enhetstester, komponenttester samt HW/SW-integrationstester.
* Introduktion till testramverket `yrgo::test`.

---

## Mål med lektionen
* Kunna redogöra för varför mjukvarutestning är viktigt, särskilt i inbyggda system.
* Känna till testpyramiden och kunna beskriva skillnaden mellan enhetstester, komponenttester och integrationstester.
* Kunna skriva och köra ett första enkelt test med `yrgo::test`.

---

## Instruktioner

### Innan lektionen
* Läs igenom kursinformationen [här](../../info/README.md).
* Repetera gärna er kodbas från **P02**; ni kommer skriva tester för denna kodbas (samt för tillagda delar i maskininlärningskursen) i projektet **P04**.
* Läs [bilaga A](./appendix/a_unit_tests.md) för en introduktion till enhetstester.

### Under lektionen
* Delta aktivt på genomgången.
* Genomför övningsuppgifterna i [bilaga B](./appendix/b_exercises.md):
  * Ni får tid på er att genomföra uppgifterna enskilt.
  * Uppgifterna gås sedan igenom i helklass.
* Lösningförslag finns [här](./exercises/README.md).

---

## Utvärdering
* Vad är skillnaden mellan ett enhetstest, ett komponenttest och ett integrationstest?
* Varför blir en bugg generellt sett dyrare att åtgärda ju senare den upptäcks?
* Vilka makron använder ni för att definiera ett testfall respektive verifiera ett förväntat resultat i `yrgo::test`?

---

## Nästa lektion
* Enhetstester (del I): enhetstester för GPIO-drivern i ATmega328p-övningsbiblioteket.
* Introduktion till kursens projekt, **P04**.

---
