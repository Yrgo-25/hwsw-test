# Bilaga A - Enhetstester

![](./images/unit_test1.png)

## Vad är ett enhetstest?
Ett **enhetstest** (*unit test*) verifierar den minsta rimliga testbara enheten i en kodbas,
oftast en enskild funktion eller en enskild klass, i **isolation** från resten av systemet.

Enhetstester utgör basen i testpyramiden som visas i figuren ovan; de är flest till antalet,
snabbast att köra och billigast att skriva och underhålla, jämfört med komponent- och
integrationstester.

Ett enhetstest svarar på en enkel fråga:

> Gör den här lilla biten kod vad den ska, givet den här indatan?

Det är alltså inte enhetstestets uppgift att verifiera att hela systemet fungerar ihop; det
kommer vi öva på med komponenttester senare i kursen (**L08–L09, L11**).

---

## Varför skriva enhetstester?
* **Snabb feedback.** Ett enhetstest tar millisekunder att köra. Att manuellt flasha en
  `ESP32-S3` och trycka på en knapp för att verifiera samma sak tar avsevärt längre tid.
* **Trygg refaktorering.** Med en testsvit på plats vågar man ändra och förbättra kod utan att
  vara rädda för att omedvetet göra sönder något som redan fungerade.
* **Levande dokumentation.** Ett välskrivet test visar hur en klass är tänkt att användas och
  vilket beteende som förväntas, ofta tydligare än en kommentar.
* **Färre buggar i produktion.** Ju tidigare ett fel upptäcks, desto billigare är det att
  åtgärda; att hitta en bugg i ett enhetstest är betydligt billigare än att hitta den när
  produkten redan levererats till kund.

---

## Struktur: Arrange-Act-Assert
De allra flesta enhetstester följer samma tydliga struktur, ofta förkortad **AAA**:
1. **Arrange**: sätt upp det som behövs för testet, t.ex. skapa objekt och förbered indata.
2. **Act**: utför själva handlingen som ska testas, t.ex. anropa en metod eller funktion.
3. **Assert**: verifiera att resultatet blev det förväntade.

```cpp
TEST(Counter, IncrementIncreasesValueByOne)
{
    // Arrange: create and initialize a counter.
    Counter counter{};

    // Act: invoke the method to test.
    counter.increment();

    // Assert: verify the expected result.
    EXPECT_EQ(counter.value(), 1U);
}
```

Att konsekvent följa AAA gör tester enkla att läsa, även för någon som inte skrivit dem, och gör
det tydligt exakt vad som failade om testet misslyckas.

---

## Grunderna i `yrgo::test`
Kursens tester skrivs med testramverket [`yrgo::test`](../../../libs/test/README.md). De två
byggstenarna ni behöver är:

* `TEST(suite, name) { ... }`: definierar ett testfall. `suite` grupperar besläktade tester
  (vanligtvis namnet på klassen som testas), `name` beskriver vad just det testfallet verifierar.
* `EXPECT_*(...)`: verifierar ett förväntat resultat. Ett testfall kan innehålla flera
  `EXPECT_*`-anrop, men fokusera helst på ett tydligt avgränsat beteende per test.

| Makro | Används för |
|---|---|
| `EXPECT_TRUE(expr)` | Verifiera att ett uttryck är sant. |
| `EXPECT_FALSE(expr)` | Verifiera att ett uttryck är falskt. |
| `EXPECT_EQ(a, b)` | Verifiera att `a` och `b` är lika. |
| `EXPECT_NE(a, b)` | Verifiera att `a` och `b` inte är lika. |
| `EXPECT_NEAR(a, b, tol)` | Verifiera att `a` och `b` ligger inom ett toleransvärde, används för flyttal. |

**OBS!** I `yrgo::test` kastar ett misslyckat `EXPECT_*` ett undantag, som ramverket fångar och
rapporterar. Det innebär att **resten av testfallet hoppas över** vid första misslyckade
verifiering; övriga testfall körs som vanligt. Om ni är vana vid GoogleTest, där `EXPECT_*`
fortsätter och bara `ASSERT_*` avbryter, är det här skillnaden att hålla reda på. Det är
ytterligare ett skäl att hålla ett tydligt avgränsat beteende per testfall.

Av samma anledning ska en hjälpfunktion som innehåller `EXPECT_*` **inte** markeras `noexcept`,
och ett `EXPECT_*` som körs i en egen `std::thread` måste fångas och kastas om på huvudtråden
(se `Transmit`-testet i **L03**). I båda fallen avslutas annars hela testprogrammet direkt, utan
att tala om vilken verifiering som brast.

Ett komplett litet exempel:

```cpp
#include "yrgo/test/test.h"

/**
 * @brief Test that adding two positive integers gives the expected sum.
 */
TEST(Math, Addition)
{
    EXPECT_EQ(2U + 2U, 4U);
}

/**
 * @brief Test entry point.
 *
 * @return 0 if all tests succeeded, -1 otherwise.
 */
int main()
{
    return yrgo::test::runAllTests() ? 0 : -1;
}
```

---

## Vad gör ett enhetstest *bra*?
* **Testar en sak i taget.** Ett test som verifierar tre orelaterade beteenden på en gång är
  svårt att felsöka när det failar: vilket av de tre gick fel?
* **Deterministiskt.** Samma test ska ge samma resultat varje gång det körs, oavsett i vilken
  ordning testerna körs eller vilken dator de körs på.
* **Snabbt.** Inga `sleep()`-anrop, ingen väntan på riktig hårdvara, ingen nätverkstrafik. Ett
  enhetstest ska ta mikro- till millisekunder.
* **Oberoende.** Tester ska inte förutsätta att ett annat test redan har körts, och de ska inte
  dela på muterbart tillstånd mellan varandra.
* **Läsbart.** Testnamnet (`suite` + `name`) ska själv beskriva vad som testas, så att en
  misslyckad testkörning direkt talar om vad som gått fel, innan man ens läser koden.

---

## Isolation och hårdvara
En viktig konsekvens av att enhetstester ska vara snabba och deterministiska; de ska inte bero
på fysisk hårdvara. Om en klass i sin nuvarande form kräver en riktig `ESP32-S3` för att
överhuvudtaget gå att instansiera går den inte att enhetstesta på host-maskinen.

Lösningen, som ni redan stöter på i övningsuppgiften nedan och som vi fördjupar oss i under
**L06**, är att koda mot ett **interface** och låta testerna använda en enkel **stubb**: en
alternativ implementation som efterliknar hårdvarans beteende med vanliga variabler i stället
för riktiga register eller pinnar. Produktionskoden (systemlogiken) känner bara till interfacet
och märker aldrig skillnaden.

---
