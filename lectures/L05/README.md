# L05 - CI-uppstart

## Dagordning
* Varför automatisera testkörning och kontroller, i stället för att lita på att någon kör dem?
* Genomgång av en `ci.yml`, skriven rad för rad live: triggers, jobb och steg.
* Demo: en pipeline som kontrollerar kodformatteringen med `clang-format`, bygger en minimal
  "firmware" och laddar upp den som artefakt.
* Från demo till eget repo: jobbet som bygger och kör **er egen** testsvit vid varje push.
* `runs-on:`: GitHub-hostade runners kontra en egen, self-hosted runner med egen etikett, samt vad
  som händer med ett jobb som ingen maskin matchar.
* Resten av lektionen: att få pipelinen grön i det egna repot, samt fortsatt arbete med **P04**.

---

## Mål med lektionen
* Kunna förklara varför en testsvit som körs automatiskt skyddar mot regressioner på ett sätt som
  en manuellt körd testsvit inte gör.
* Kunna skriva en `ci.yml` med jobb för formatteringskontroll, firmware-bygge med
  artefaktuppladdning samt körning av testsviten.
* Kunna förklara vad `needs:` gör, och när det är rimligt att låta ett jobb vänta på ett annat.
* Kunna förklara varför en artefakt måste laddas upp från samma jobb som byggde den.
* Kunna köra `clang-format` mot sin kodbas och förklara vad en delad formatteringskonfiguration
  löser.
* Ha en grön pipeline i gruppens eget repo, som körs vid varje push och pull request.
* Veta hur byggsteget byts ut mot ESP-IDF för att bygga egen firmware i **P04**.
* Kunna förklara vad `runs-on:` väljer, och känna igen symptomet när ingen runner matchar.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_ci_startup.md) om CI-uppstart.

### Under lektionen
* Delta aktivt på genomgången, där pipelinen från [bilaga A](./appendix/a_ci_startup.md)
  demonstreras live.
* Genomför därefter övningsuppgifterna i [bilaga B](./appendix/b_exercises.md), i grupp och i ert
  eget repo.
* **Till skillnad från tidigare lektioner gås lösningen inte igenom i helklass efteråt.** Att
  sätta upp gruppens pipeline är projektarbete i **P04**: varje grupps repo ser olika ut, och det
  finns därför ingen gemensam lösning att visa på tavlan. Demot i bilaga A är mallen ni utgår
  från, och jag går runt och hjälper till under tiden. Fråga hellre en gång för mycket.
* Notera att testsviten som ska köras i **er** pipeline är er egen, för era `ESP32-S3`-drivers och
  `ml::lin_reg::Adaptive` i **P04**, inte de tester ni skrivit mot ATmega328p-övningsbiblioteket
  under **L02–L04**.
* Resterande tid ägnas åt att få pipelinen grön samt åt fortsatt arbete med **P04**.

---

## Utvärdering
* Vad skyddar en automatiskt körd testsvit mot, som en manuellt körd inte gör?
* Vilka jobb innehåller er pipeline, och varför är de separata jobb i stället för ett enda?
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
