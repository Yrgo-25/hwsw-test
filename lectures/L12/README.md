# L12 - Statisk analys och verktyg för korrekthet

## Dagordning
* Statisk analys: att hitta buggar utan att köra koden.
* Demo: verktygen körda på ett minimalt program, där felet är känt i förväg.
* Verktyg: `clang-tidy` och `cppcheck`.
* Körtidsanalys med `AddressSanitizer (ASan)` och `UndefinedBehaviorSanitizer (UBSan)`.
* Att låta pipelinen växa: statisk analys och sanitizers som egna jobb i den `ci.yml` ni satte
  upp i **L05**.
* Fortsatt arbete med **P04**.

---

## Mål med lektionen
* Kunna förklara skillnaden mellan statisk analys och testning (körning av kod).
* Kunna köra `clang-tidy`/`cppcheck` på en kodbas och tolka resultatet.
* Kunna köra en testsvit med ASan/UBSan och tolka en sanitizer-rapport.
* Kunna avgöra när en varning kan ignoreras och när den bör åtgärdas.
* Kunna lägga till ett nytt jobb i en befintlig pipeline, och motivera varför analysen hör hemma
  där snarare än enbart på den egna datorn.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_static_analysis.md) om statisk analys och sanitizers.

### Under lektionen
* Delta aktivt på genomgången, där verktygen demonstreras live på ett minimalt program med en
  medvetet inplanterad bugg.
* Genomför därefter övningsuppgifterna i [bilaga B](./appendix/b_exercises.md), i grupp och i ert
  eget repo.
* **Lösningen gås inte igenom i helklass efteråt.** Att tillämpa det här på gruppens egen kodbas
  är projektarbete i **P04**: varje grupps repo ser olika ut, och det är i genomförandet ni lär
  er tekniken. Demot från genomgången är mallen ni utgår från, och jag går runt och hjälper till
  under tiden.

---

## Utvärdering
* Vad kan statisk analys hitta som era enhets- och komponenttester inte kan, och vice versa?
* Hittade sanitizers eller den statiska analysen något i er kodbas? Vad, och hur åtgärdade ni
  det?
* Varför är det värt att köra analysen i pipelinen, när ni ändå kan köra den lokalt?

---

## Nästa lektion
* Testautomatisering och kontinuerlig integration (CI).
* Fortsatt arbete med **P04**.

---
