# Bilaga B - Övningsuppgifter

Uppgifterna nedan görs i grupp och i **ert eget repo**, och bygger vidare på mätningen från
**L16**. Tillsammans med den utgör de mätdatadelen av G-kravet
[Mätdatainsamling och automatiserade integrationstester](../../../projects/P04/README.md#mätdatainsamling-och-automatiserade-integrationstester-g)
i **P04**. De gås inte igenom i helklass; läraren går runt och hjälper till under tiden.

---

## Krav
* Formulera det krav er mätning ska jämföras mot, med nominellt värde och tolerans (se
  [bilaga A](./a_processing_visualization.md)).
* Dokumentera kravet i ert repo innan ni analyserar mätdatan, tillsammans med var det kommer
  ifrån: er kravställning från **P02**/**P03**, eller ert eget motiverade val.

---

## Processning
* Skriv ett Python-skript som läser in er CSV-fil och räknar fram de storheter kravet handlar om,
  t.ex. medelvärde, standardavvikelse och största avvikelse.
* Låt skriptet ta CSV-filen som argument, så att samma skript kan köras på varje ny mätning.

---

## Visualisering
* Rita upp mätdatan med `matplotlib`, med kravet inritat i samma graf, och spara grafen till fil.
* Avgör utifrån grafen om systemet uppfyller kravet, och motivera bedömningen.
* Checka in grafen bredvid mätdatan.

---

## Automatisk bedömning
* Låt skriptet skriva ut `PASS` eller `FAIL` och avsluta med returkod `0` respektive `1`.
* Kontrollera att skriptet faktiskt kan bli underkänt: skärp toleransen tillfälligt, eller mata
  in en CSV-fil med ett påhittat mätvärde utanför kravet, och kör skriptet igen.

---

## Diskussion
* Stämmer skriptets bedömning med den ni gjorde utifrån grafen? Om inte, vilken av dem har fel?
* Är ert krav rimligt satt, givet den spridning ni mätte upp? Vad hade hänt med bedömningen om ni
  bara hade tittat på medelvärdet?

---
