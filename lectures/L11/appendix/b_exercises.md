# Bilaga B - Övningsuppgifter

## Mutationstestning
Välj ut tre av era befintliga tester och kontrollera att varje test innehåller minst en `EXPECT_*` som faktiskt verifierar det beteende som testet är avsett att kontrollera:
* Fundera på om denna assertion skulle misslyckas om implementationen blev fel.
* Genomför den manuella mutationstestningen från bilaga A på en valfri liten funktion i er kodbas. Failade något test? Om inte, komplettera testerna så att de gör det.

---

## Förbättra ett test
Välj ett befintligt test som innehåller få eller inga verifieringar: 
* Förbättra det genom att lägga till lämpliga `EXPECT_*`-satser. 
* Motivera varför de nya verifieringarna gör testet bättre.

---

## Fördjupningsuppgift: Generera en täckningsrapport med `gcov`/`lcov`
Installera `gcov`/`lcov` (`sudo apt -y install lcov`, `gcov` följer med `gcc`) och generera en
täckningsrapport för er testsvit. Kom ihåg att koden måste **byggas om** med `--coverage`;
det räcker inte att köra verktygen på en färdig binär. Kommandona finns i
[bilaga A](./a_code_coverage.md#så-genererar-ni-en-rapport).

Vill ni prova på övningsbibliotekets testsvit först går det via `EXTRA_FLAGS`:

```bash
cd libs/atmega/test
make clean && make EXTRA_FLAGS="--coverage -O0 -g"
lcov --capture --directory . --output-file cov.info
genhtml cov.info --output-directory cov_html
```

Identifiera två kodområden som saknar täckning:
* Behöver de testas, eller finns det en rimlig anledning att de inte körs?
* Finns det kodområden som täcks av tester men där resultatet verifieras bristfälligt?

---

## Diskussion
* Kan en testsvit ha 100 % radtäckning och ändå missa en allvarlig bugg? Ge ett exempel.
* Är det värt tiden att sträva efter 100 % täckningsgrad i ett studentprojekt som **P04**?
  Motivera.

---
