# Bilaga A - Självgranskning av testsviten inför redovisning
Använd den här checklistan för att stämma av hur långt er testsvit har kommit inför finputsningen i **L19**. Den speglar de individuella bedömningskriterierna i 
[P04](../../../projects/P04/README.md#bedömning).

---

## Enhetstester
* Har ni enhetstester för era riktiga `Esp32s3`-drivers, skrivna mot mockade ESP-IDF-funktioner
  snarare än fysisk hårdvara (se **L06**), med minst en driver per gruppmedlem?
* Har ni enhetstester för `driver::tempsensor::Tmp36`s temperaturformel?
* Har ni enhetstester för `ml::lin_reg::Adaptive` och `driver::tempsensor::Smart` från **P03**s
  Fas 1, samt för den algoritm ni valde i Fas 2?
* Innehåller testerna gränsfall, inte bara typiska värden (se **L09**)?

## Komponenttester
* Har ni komponenttester som verifierar samspelet mellan flera klasser, med stubbar för
  otestade beroenden (se **L08**)?
* Kan ni, för varje komponenttest, förklara exakt vilken klass som är riktig och vilka som är
  stubbade, och varför?

## Testautomatisering och kodkvalitet
* Bygger och kör en CI-pipeline hela testsviten automatiskt vid push (se **L13**)?
* Har ni kört statisk analys och/eller sanitizers mot kodbasen, och åtgärdat det som hittades
  (se **L12**)?
* Följer testkoden en tydlig struktur, en testsvit per testad klass?

## Individuellt bidrag
* Kan ni, oberoende av resten av gruppen, peka på specifika tester i Git-historiken som ni
  själva skrivit?
* Kan ni förklara vad varje sådant test verifierar, vilken testnivå det tillhör, och varför det
  är utformat som det är, utan att titta i koden?

---

## Om något saknas
Är svaret nej på någon punkt ovan, är det precis det ni bör prioritera under **L18–L19**, inte
nya funktioner eller kosmetiska förbättringar.

---
