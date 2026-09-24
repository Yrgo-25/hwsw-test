# L13 - Kodkvalitet i pipelinen: statisk analys, branch protection och jobbordning

## Dagordning
* Kort demo: `clang-tidy` och `cppcheck` körda på ett minimalt program, där felet är känt i
  förväg.
* Branch protection, så att ett rött jobb faktiskt hindrar en merge.
* Ordningen mellan jobben i pipelinen, nu när testsviten vuxit (se **L05**–**L06**).
* Fortsatt arbete med **P04**.

---

## Mål med lektionen
* Kunna köra `clang-tidy`/`cppcheck` på en kodbas, tolka resultatet och avgöra när en varning
  bör åtgärdas.
* Ha branch protection konfigurerad för `main` i ert eget repo, i enlighet med kraven i **P04**.
* Kunna motivera var testjobbet hör hemma i kedjan, utifrån hur lång tid jobben tar.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_code_quality.md) om statisk analys, branch protection och
  jobbordning.
* Ta fram gruppens egen `.github/workflows/ci.yml` från **L05**, ni utgår från den under
  lektionen.

### Under lektionen
* Delta aktivt på demot.
* Genomför därefter uppgifterna i [bilaga B](./appendix/b_exercises.md), i grupp och i ert eget
  repo. Demot är mallen ni utgår från, och jag går runt och hjälper till under tiden.
* Resterande tid: fortsatt arbete med komponenttesterna i **P04**.

---

## Utvärdering
* Hittade den statiska analysen något i er kodbas? Vad, och hur åtgärdade ni det?
* Vad hindrar nu någon i gruppen från att merga en pull request med rött kryss?

---

## Nästa lektion
* Projektarbete (del I).

---
