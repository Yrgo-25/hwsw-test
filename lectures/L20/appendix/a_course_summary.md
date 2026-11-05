# Bilaga A - Sammanfattning av kursen
En kompakt repetition av kursens centrala begrepp, som stöd inför redovisningen av **P04** i
**L20**. Den är även användbar inför praktiskt prov 2, som examineras under Maskininlärning-kursens
**L20**;
den kompletterar, men ersätter inte,
[hemtentamen 2](https://github.com/Yrgo-25/machine-learning/blob/main/exams/exam2/home_exam2.md),
uppgift 1, 2 och 4.

---

## Testnivåer
* **Enhetstest.** Testar en enskild klass i isolation, snabbt och deterministiskt, utan fysisk
  hårdvara (**L01–L07**).
* **Komponenttest.** Testar hur flera klasser samverkar, med stubbar för beroenden som inte
  testas direkt (**L08–L09, L11–L12**).
* **Test med riktiga lager/klasser hela vägen.** Tränar t.ex. ett helt nätverk på riktig data och
  kontrollerar slutresultatet; hittar fel i hur korrekta delar samverkar, t.ex. en gradient som
  skickas vidare i fel ordning eller med fel dimension mellan två lager som var för sig är
  korrekta (praktiskt prov 2).
* **HW/SW-integrationstest.** Verifierar det färdiga systemet på riktig hårdvara, automatiserat med
  `pytest` via seriell kommunikation (**L18**) eller manuellt och spårbart dokumenterat (**L19**).

## Struktur och verktyg
* **Arrange-Act-Assert**, samt `TEST`/`EXPECT_*`-makrona i `yrgo::test` (**L01**).
* **Stubbar** som en enkel, alternativ interface-implementation (**L01**, **L08–L09**), gärna med
  styrbar utdata så att testet kan mata in kända värden (**L11**), samt mockning av ESP-IDF:s
  C-funktioner för att enhetstesta drivers egen logik (**L06**).
* **Testbar design**: dependency injection, koda mot interface snarare än konkret klass
  (**L06**).

## Kvalitet och verktygskedja
* **Gränsvärdesanalys**: testa vid, precis under och precis över en gräns (**L12**).
* **Ett test måste kunna bli rött**: det ska innehålla en `EXPECT_*` som skulle misslyckas om
  koden vore fel.
* **Statisk analys** (`clang-tidy`, `cppcheck`) hittar misstänkta mönster utan att köra koden
  (**L13**). **Sanitizers** (ASan/UBSan) hittar minnesfel och odefinierat beteende vid körning
  (**L12**).
* **CI**: hela testsviten byggs och körs, och firmwaren för `ESP32-S3` byggs via ESP-IDF, automatiskt vid varje push och pull request (**L05–L06**), och branch protection gör att röda jobb blockerar en merge (**L13**).

## Mätdata
* Ett väldefinierat protokoll för att samla in mätdata från en DUT, samt strukturerad lagring
  t.ex. som CSV (**L16**).
* Ett mätbart krav med nominellt värde och tolerans, processning av mätdatan, visualisering med
  kravet inritat samt en automatisk bedömning med returkod (**L17**).

---

## Självkontroll
* Kan ni, utan att titta i anteckningarna, förklara skillnaden mellan ett enhetstest och ett
  komponenttest, och varför man använder stubbar i det senare?
* Kan ni motivera varför `EXPECT_NEAR` används i stället för `EXPECT_EQ` för flyttalsresultat?
* Kan ni beskriva vad en stubb är, och hur den skiljer sig från den riktiga implementationen den
  ersätter i ett test?
* Kan ni förklara varför ett komponenttest normalt inte använder den riktiga implementationen av alla beroenden?
* Kan ni förklara när ett fel ska avvisas med `false` och när ett kontrollerat avslut
  (`std::terminate()`) är rimligare, och hur det påverkar vad `yrgo::test` kan rapportera?
* Kan ni ge ett exempel på ett integrationstest som lämpar sig för automatisering, och ett som kräver manuell verifiering?
* Kan ni förklara varför ett HW/SW-integrationstest behöver ett känt utgångsläge, och vad som
  händer med testresultaten om det saknas?

---
