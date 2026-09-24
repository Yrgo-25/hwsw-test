# L12 - Komponenttester (del IV): gränsfall, robusthet och sanitizers

## Dagordning
* Gränsvärdesanalys (*boundary value analysis*) och varför edge cases hittar fler buggar än
  "typiska" indata.
* Kort genomgång: robust mjukvara, ogiltig konfiguration kontra ogiltig indata, samtidiga
  händelser samt sanitizers (ASan/UBSan).
* Gränsfall i er egen systemlogik, som en fortsättning på komponenttesterna från **L11**.
* Fortsatt arbete med komponenttesterna i **P04**.

---

## Mål med lektionen
* Kunna tillämpa gränsvärdesanalys och ekvivalenspartitionering för att välja testfall.
* Kunna identifiera relevanta gränsfall för `system::logic::Logic`, inklusive samtidiga
  händelser, och skriva komponenttester som täcker dem.
* Kunna motivera när ett fel ska avvisas med ett returvärde och när ett kontrollerat avslut är
  rimligare.
* Kunna köra testsviten med ASan/UBSan och tolka en sanitizer-rapport.
* Ha påbörjat robusthetskraven (VG) i **P04**, för den som siktar på VG.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_boundary_analysis.md) om gränsvärdesanalys.
* Läs [bilaga B](./appendix/b_robustness.md) om robust mjukvara och sanitizers.

### Under lektionen
* Delta aktivt på genomgången.
* Genomför därefter uppgifterna i [bilaga C](./appendix/c_exercises.md), i grupp och i ert eget
  repo. De gås inte igenom i helklass; jag går runt och hjälper till under tiden.

---

## Utvärdering
* Vilka gränsfall identifierade ni, och hittade något av dem ett fel i er implementation?
* Hittade ASan/UBSan något i er testsvit? Vad, och hur åtgärdade ni det?

---

## Nästa lektion
* Kodkvalitet i pipelinen: statisk analys, branch protection och jobbordning.
* Fortsatt arbete med **P04**.

---
