# Bilaga C - Självgranskning av testsviten inför redovisning
Använd den här checklistan under **L19** för att stämma av hur långt projektet har kommit inför
redovisningen i **L20**. Den speglar kraven och de individuella bedömningskriterierna i
[P04](../../../projects/P04/README.md#bedömning).

---

## Enhetstester
* Har ni enhetstester för era riktiga `Esp32s3`-drivers, skrivna mot mockade ESP-IDF-funktioner
  snarare än fysisk hårdvara (se **L06**), med minst en driver per gruppmedlem?
* Har ni enhetstester för `driver::tempsensor::Tmp36`s temperaturformel?
* Har ni enhetstester för `ml::lin_reg::Adaptive` och `driver::tempsensor::Smart` från **P03**s
  Fas 1, samt för den algoritm ni valde i Fas 2 (se **L07**, **L11**)?
* Innehåller testerna gränsfall, inte bara typiska värden (se **L12**)?
* Blir varje test rött om koden det verifierar går sönder? Prova genom att medvetet ändra en rad
  i produktionskoden, t.ex. byta `<` mot `<=`, och kör testsviten.

## Komponenttester
* Har ni komponenttester som verifierar samspelet mellan flera klasser, med stubbar för
  otestade beroenden (se **L11**)?
* Kan ni, för varje komponenttest, förklara exakt vilken klass som är riktig och vilka som är
  stubbade, och varför?

## Testautomatisering och kodkvalitet
* Bygger och kör en CI-pipeline hela testsviten automatiskt vid push och pull request (se
  **L05–L06**), och hindrar branch protection en merge med röda jobb (se **L13**)?
* Kontrollerar pipelinen kodformatteringen, och bygger den er firmware för `ESP32-S3` samt
  laddar upp binären som en artefakt (se **L05**)?
* Har ni kört statisk analys mot kodbasen, och åtgärdat det som hittades (se **L13**)?
* (VG) Har ni kört testsviten med sanitizers, och åtgärdat det som hittades (se **L12**)?
* Följer testkoden en tydlig struktur, en testsvit per testad klass?

## Mätdatainsamling och integrationstester
* Har ni ett dokumenterat protokoll för mätdata, och ett Python-skript som samlar in mätdata från
  `ESP32-S3` och sparar den som CSV (se **L16**)?
* Har ni ett mätbart krav, och en graf där mätdatan visualiseras med kravet inritat (se **L17**)?
* Har ni minst ett automatiserat integrationstest i `pytest` som går igenom mot riktig hårdvara,
  och som ni har sett bli rött (se **L18**)?

## VG
* *Säker och robust programvara:* har ni testat minst tre gränsfall utöver G-testerna, och kört
  testsviten med sanitizers (se **L12**)?
* *Ett testramverk för kontinuerlig testning:* har ni, om ni valt den vägen, en automatisk
  bedömning av mätdata mot kravställning, integrationstester för samtliga seriella kommandon med
  testrapport (se **L17–L18**), samt minst ett dokumenterat manuellt integrationstest (se
  **L19**)?

## Rapport
* Innehåller `p04_report.md` fördelningen av drivers och övriga bidrag inom gruppen, samt var och
  hur AI-verktyg har använts?

## Individuellt bidrag
* Kan ni, oberoende av resten av gruppen, peka på specifika tester i Git-historiken som ni
  själva skrivit?
* Kan ni förklara vad varje sådant test verifierar, vilken testnivå det tillhör, och varför det
  är utformat som det är, utan att titta i koden?

---

## Om något saknas
Är svaret nej på någon punkt ovan, är det precis det ni bör prioritera under resten av **L19**, inte
nya funktioner eller kosmetiska förbättringar.

---
