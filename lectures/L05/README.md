# L05 - CI-uppstart

## Dagordning
* **P04** i sin helhet: krav per betygsnivå, fördelning av drivers inom gruppen, bidragsrapport,
  redovisning samt regler för AI-verktyg. Härifrån och framåt arbetar ni i er egen kodbas.
* Varför automatisera testkörning och kontroller, i stället för att lita på att någon kör dem?
* Genomgång av en `ci.yml`, skriven rad för rad live: triggers, jobb och steg.
* Demo: en pipeline som kontrollerar kodformatteringen med `clang-format`, bygger en minimal
  "firmware" och laddar upp den som artefakt.
* Från demo till eget repo: samma pipeline i er egen kodbas, plus testramverk och testbygge på
  plats, förberett för testjobbet i **L06**.
* `runs-on:`: GitHub-hostade runners kontra en egen, self-hosted runner med egen etikett, samt vad
  som händer med ett jobb som ingen maskin matchar.
* Resten av lektionen: att få pipelinen grön i det egna repot, samt fortsatt arbete med **P04**.

---

## Mål med lektionen
* Kunna förklara varför en testsvit som körs automatiskt skyddar mot regressioner på ett sätt som
  en manuellt körd testsvit inte gör.
* Kunna skriva en `ci.yml` med två jobb: formatteringskontroll samt firmware-bygge med
  artefaktuppladdning.
* Kunna förklara vad `needs:` gör, och när det är rimligt att låta ett jobb vänta på ett annat.
* Kunna förklara varför en artefakt måste laddas upp från samma jobb som byggde den.
* Kunna köra `clang-format` mot sin kodbas och förklara vad en delad formatteringskonfiguration
  löser.
* Ha en grön pipeline i gruppens eget repo, som körs vid varje push och pull request.
* Veta vad **P04** kräver av er, både som grupp och individuellt, och vem i gruppen som ansvarar
  för vilken driver.
* Veta hur byggsteget byts ut mot ESP-IDF för att bygga egen firmware i **P04**.
* Kunna förklara vad `runs-on:` väljer, och känna igen symptomet när ingen runner matchar.

---

## Instruktioner

### Innan lektionen
* Läs igenom projektbeskrivningen för [P04](../../projects/P04/README.md) i sin helhet, inklusive
  krav och bedömningskriterier. Ta med frågor till lektionen.
* Läs [bilaga A](./appendix/a_ci_startup.md) om CI-uppstart.

### Under lektionen
* Delta aktivt på genomgången: först **P04** i sin helhet, därefter pipelinen från
  [bilaga A](./appendix/a_ci_startup.md), som demonstreras live.
* Genomför därefter övningsuppgifterna i [bilaga B](./appendix/b_exercises.md), i grupp och i ert
  eget repo.
* **Till skillnad från tidigare lektioner gås lösningen inte igenom i helklass efteråt.** Att
  sätta upp gruppens pipeline är projektarbete i **P04**: varje grupps repo ser olika ut, och det
  finns därför ingen gemensam lösning att visa på tavlan. Demot i bilaga A är mallen ni utgår
  från, och jag går runt och hjälper till under tiden. Fråga hellre en gång för mycket.
* Notera att pipelinen ännu inte kör några tester. Era egna tester börjar skrivas i **L06**, och
  det är då testjobbet läggs till. De tester ni skrivit under **L02–L04** hör till kursens
  ATmega328p-övningsbibliotek och ligger inte i er egen kodbas.
* Resterande tid ägnas åt att få pipelinen grön samt åt fortsatt arbete med **P04**.

---

## Utvärdering
* Vad skyddar en automatiskt körd testsvit mot, som en manuellt körd inte gör?
* Vem i gruppen ansvarar för vilken driver, och hur dokumenterar ni det?
* Vilka två jobb innehåller er pipeline, och varför är de separata jobb i stället för ett enda?
* Vad hade hänt om ni tog bort `needs:` från firmware-jobbet? När är det en förbättring, och när
  är det en försämring?
* Varför måste artefakten laddas upp från samma jobb som byggde den?
* Vad har ni för nytta av att den byggda binären finns kvar efter att körningen är klar?
* Varför är det värdefullt att hela gruppen använder samma `clang-format`-konfiguration, i
  stället för att var och en formatterar sin kod efter eget tycke?
* Ett jobb blir stående i `Queued` utan felmeddelande. Vad är den troligaste orsaken?

---

## Nästa lektion
* Testbar design, stubbar och virtuell hårdvara.
* Fortsatt arbete med **P04**.

---
