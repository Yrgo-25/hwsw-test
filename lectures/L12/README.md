# L12 - Statisk analys och verktyg för korrekthet

## Dagordning
* Statisk analys: att hitta buggar utan att köra koden.
* Verktyg: `clang-tidy` och `cppcheck`.
* Körtidsanalys med `AddressSanitizer (ASan)` och `UndefinedBehaviorSanitizer (UBSan)`.
* Fortsatt arbete med **P04**.

---

## Mål med lektionen
* Kunna förklara skillnaden mellan statisk analys och testning (körning av kod).
* Kunna köra `clang-tidy`/`cppcheck` på en kodbas och tolka resultatet.
* Kunna köra en testsvit med ASan/UBSan och tolka en sanitizer-rapport.
* Kunna avgöra när en varning kan ignoreras och när den bör åtgärdas.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_static_analysis.md) om statisk analys och sanitizers.

### Under lektionen
* Delta aktivt på genomgången.
* Genomför övningsuppgifterna i [bilaga B](./appendix/b_exercises.md):
  * Ni får tid på er att genomföra uppgiften enskilt eller i grupp.
  * Uppgiften gås sedan igenom i helklass.

---

## Utvärdering
* Vad kan statisk analys hitta som era enhets- och komponenttester inte kan, och vice versa?
* Hittade sanitizers eller den statiska analysen något i er kodbas? Vad, och hur åtgärdade ni
  det?

---

## Nästa lektion
* Testautomatisering och kontinuerlig integration (CI).
* Fortsatt arbete med **P04**.

---
