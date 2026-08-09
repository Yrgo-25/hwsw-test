# Bilaga A - Täckningsgrad och testkvalitet

## Vad mäter täckningsgrad?
**Täckningsgrad** (*code coverage*) mäter hur stor andel av produktionskoden som *körs* när
testsviten körs, ofta uttryckt som radtäckning (andel körda kodrader) eller grentäckning (andel
körda `if`/`else`-grenar). Verktyg som `gcov`/`lcov` kan generera sådana rapporter för C++-kod.

Det låter förtroendeingivande med en hög siffra, men täckningsgrad mäter bara att koden
exekverades, inte att resultatet faktiskt verifierades.

### Så genererar ni en rapport
Täckningsgrad kräver att koden kompileras med **instrumentering**; det räcker inte att köra
`gcov` på en färdig binär. Bygg om testsviten med `--coverage` (som är kortform för
`-fprofile-arcs -ftest-coverage`) och utan optimering, kör den, och sammanställ sedan resultatet:

```bash
cd libs/atmega/test
make clean && make EXTRA_FLAGS="--coverage -O0 -g"   # bygger och kör testsviten
lcov --capture --directory . --output-file cov.info
genhtml cov.info --output-directory cov_html         # öppna cov_html/index.html
```

Grentäckning räknas inte med som standard; lägg till `--rc branch_coverage=1` till både `lcov`
och `genhtml` för att få med den.

---

## Fällan: exekvering utan verifiering
Ett test kan anropa en funktion och därmed räknas som att ha "täckt" den koden i en
täckningsrapport, utan att en enda `EXPECT_*` verifierar vad funktionen faktiskt returnerade.
Ett sådant test skulle inte upptäcka om funktionen slutade fungera i morgon.

```cpp
// Ger 100 % radtäckning på increment(), men verifierar ingenting.
TEST(Counter, IncrementRunsWithoutCrashing)
{
    Counter counter{};
    counter.increment();
}

// Täcker samma kod, men verifierar faktiskt att resultatet blev korrekt.
TEST(Counter, IncrementIncreasesValueByOne)
{
    Counter counter{};
    counter.increment();
    EXPECT_EQ(counter.value(), 1);
}
```

Hög täckningsgrad är alltså nödvändig, men ensam inte tillräcklig, för en bra testsvit.

---

## Ett enkelt självtest: manuell mutationstestning
Ett effektivt sätt att kontrollera om era tester verkligen verifierar något är att medvetet ändra en rad i produktionskoden så att den blir fel:
* Byt t.ex. `<` mot `<=`, eller `+` mot `-`.
* Kör testsviten och se om något test failar. 
* Om inget test failar trots att koden nu är trasig verifierar testet egentligen inte just den raden.
* Ångra ändringen efteråt.

---
