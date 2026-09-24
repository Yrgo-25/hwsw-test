# L19 - Redovisning av P04 och förberedelse inför praktiskt prov 2

## Dagordning
* Redovisning och inlämning av **P04**.
* Förberedelse inför praktiskt prov 2: genomgång av testuppgifterna i övningstentamen 2.

---

## Mål med lektionen
* Ha redovisat samt lämnat in **P04**.
* Kunna förklara skillnaden mellan ett enhetstest, ett komponenttest och ett test som tränar ett
  helt nätverk med riktiga lager, samt vad respektive testtyp kan hitta.
* Känna sig förberedd inför det praktiska provet i **L20**.

---

## Instruktioner

### Innan lektionen
* Kontrollera att projektet bygger, att testsviten går igenom och att allt material finns
  tillgängligt inför redovisningen.
* Läs igenom testuppgifterna (uppgift 6 samt 10–11) i
  [övningstentamen 2](https://github.com/Yrgo-25/Machine-Learning/blob/main/exams/exam2/practice_exam2.md).

### Under lektionen
* Redovisa **P04** enligt [Redovisning](../../projects/P04/README.md#redovisning) i
  uppgiftsbeskrivningen.
* Övriga tiden: förberedelse inför **L20**, med stöd av
  [bilaga A](./appendix/a_course_summary.md), en sammanfattning av kursens centrala begrepp.
  Tre saker från övningstentamen 2 som är värda att repetera särskilt:
  * Att komplettera en stubb med styrbar utdata, se
    [bilaga A i L11](../L11/appendix/a_controllable_stubs.md).
  * När ett fel ska avvisas med `false` och när ett kontrollerat avslut är rimligare, se
    [bilaga B i L12](../L12/appendix/b_robustness.md).
  * Vad ett test som tränar ett helt nätverk med riktiga lager kan hitta, som varken enhetstester
    eller komponenttester med stubbar kan: fel i hur lagren *samverkar*, t.ex. en gradient som
    skickas vidare i fel ordning eller med fel dimension mellan två lager som var för sig är
    korrekta.
* Ställ frågor om sådant som känns oklart inför provet.

---

## Utvärdering
* Är **P04** redovisat och inlämnat enligt kraven i uppgiftsbeskrivningen?
* Vad är ni mest nöjda med i er testsvit, och vad hade ni gjort annorlunda om ni fick börja om?
* Känner ni er redo att genomföra det praktiska provet på egen hand?

---

## Nästa lektion
* Praktiskt prov 2 (gemensamt med Maskininlärning) och kursavslut.

---
