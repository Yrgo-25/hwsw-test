# L07 - Lösningsförslag, övningsuppgifter
Lösningsförslag till övningsuppgiften i [bilaga B](../appendix/b_exercises.md): strukten
`driver::Tmp36`, tillsammans med ett testfall som jämför dess temperatur mot en referens för
samtliga ADC-värden.

---

## Innehåll

```text
exercises/
├── Makefile              - Bygger och kör testsviten.
├── include/
│   └── driver/
│       └── tmp36.h       - TMP36 stubbdriver.
└── testsuite.cpp         - Testfallet, och hjälpfunktioner.
```

---

## Bygga och köra
Testramverket ligger i git-submodulen `libs/test`. Hämta detta först, om det inte redan är gjort:

```bash
git submodule update --init --recursive
```

Kör därefter testsviten från den här mappen:

| Kommando | Beskrivning |
|---|---|
| `make` | Bygger och kör testsviten. |
| `make build` | Bygger enbart testsviten. |
| `make run` | Bygger vid behov och kör testsviten. |
| `make clean` | Tar bort testsviten samt `yrgo::test`-biblioteket. |

Testfallet ska passera:

```text
1 out of 1 test cases succeeded!
```

---

## `driver::Tmp36`
Strukten är en förenklad TMP36-driver. I stället för att läsa av en ADC tar den emot inspänningen
direkt, vilket gör att testet klarar sig utan både hårdvara och en ADC-stubb:
* `setInputVoltage()` sparar spänningen, men ignorerar värden utanför `[0.0, 5.0]` V. Den tidigare
  spänningen bibehålls då.
* `read()` räknar om spänningen till en temperatur i enlighet med databladet:
`T = (Uin - 0.5) / 0.01`.

---

## Testfallet
`TEST(Temperature, Accuracy)` går igenom samtliga 1024 ADC-värden i `[0, 1023]`. För varje värde:
1. Inspänningen räknas ut med `computeInputVoltage()` (10-bitars ADC, `Uref = 5.0 V`) och matas
   in i strukten.
2. Den förväntade temperaturen räknas ut med `convertToTemp()`, som använder den omskrivna formen
   `T = 100 * Uin - 50`.
3. Förväntad och faktisk temperatur jämförs med `EXPECT_NEAR()`.

Hjälpfunktionerna utgör alltså testets referens: en egen uppställning av samma formel, inte ett
anrop till strukten själv.

---

## `EXPECT_EQ()` kontra `EXPECT_NEAR()`
Byt `EXPECT_NEAR()` mot `EXPECT_EQ()`, och testfallet blir rött redan vid `adcVal = 8`:

```text
Test case Temperature.Accuracy failed: EXPECT_EQ(expectedTemp, actualTemp) failed: -46.0899 != -46.0899 (testsuite.cpp:56)
0 out of 1 test cases succeeded!
```

De två formerna av formeln är matematiskt identiska, men i flyttal skiljer de sig för 307 av de
1024 ADC-värdena, långt bortom de decimaler som skrivs ut. Att bara den första avvikelsen syns
beror på att `EXPECT_EQ()` kastar ett undantag, vilket avbryter testfallet.

**OBS!** Ta även bort `tol` när ni byter makro. Annars blir variabeln oanvänd, och med `-Werror`
i makefilen bygger testsviten inte alls:

```text
error: unused variable 'tol' [-Werror=unused-variable]
```

---

## Val av tolerans
Testsviten använder toleransen `1e-9`. En rimlig tolerans ligger klart över avrundningsbruset,
men klart under de fel som testet är till för att hitta. Här är vad som händer på båda sidor.

### För snäv tolerans
Den största avvikelsen mellan formerna är ca `5.7e-14`, vid `adcVal = 630`. Med en tolerans under
den, t.ex. `1e-14`, blir testet rött trots att koden är korrekt:

```text
Test case Temperature.Accuracy failed: EXPECT_NEAR(expectedTemp, actualTemp, tol) failed: |68.7683 - 68.7683| = 1.42109e-14 > 1e-14 (testsuite.cpp:56)
```

Det ligger då nära till hands att välja `1e-13`, strax över den uppmätta avvikelsen. Testet blir
grönt, men marginalen är mindre än en faktor två, och avrundningsbruset är ingen fast egenskap hos
formeln. Det beror på hur kompilatorn räknar ut uttrycken, och därmed på kompilator,
optimeringsflaggor och processor. Samma testsvit byggd med `-O2 -ffast-math` ger till exempel
avvikelser för 225 ADC-värden i stället för 307, och den första redan vid `adcVal = 1`. En
tolerans som är inställd efter ett enda bygge kan alltså bli röd i ett annat, utan att koden har
ändrats. Ett sådant test har samma problem som `EXPECT_EQ()`, bara i mindre skala.

### För generös tolerans
Varför inte välja `1e-2`, som också blir grönt? För att en så generös tolerans döljer verkliga fel.
Skriv till exempel `voltagePerDegree_v{0.01f}` i stället för `voltagePerDegree_v{0.01}` i `read()`.
Konstanten avrundas då till `float`-precision, och temperaturen avviker med upp till `1e-5` grader.
Med `1e-2` passerar testet ändå, med `1e-9` blir det rött direkt:

```text
Test case Temperature.Accuracy failed: EXPECT_NEAR(expectedTemp, actualTemp, tol) failed: |-50 - -50| = 1.11759e-06 > 1e-09 (testsuite.cpp:56)
```

### Lagom tolerans
`1e-9` ligger drygt fyra tiopotenser över avrundningsbruset, så testet tål att bruset varierar
mellan byggen. Samtidigt fångar den både `float`-felet ovan, med avvikelser runt `1e-6`, och
gränsfelet i nästa avsnitt, med en avvikelse på ca `0.49` grader.

---

## Därför ett helt intervall
Ett test med ett enda, typiskt värde hade missat ett lättgjort fel. Anta att övre gränsen i
`setInputVoltage()` blir `1.0` i stället för `5.0` V, samma gräns som för en lärhastighet. Vid
`adcVal = 154` (0.7527 V) märks ingenting, men testet över hela intervallet fastnar vid första
värdet över 1.0 V:

```text
Test case Temperature.Accuracy failed: EXPECT_NEAR(expectedTemp, actualTemp, tol) failed: |50.1955 - 49.7067| = 0.488759 > 1e-09 (testsuite.cpp:56)
```

`adcVal = 205` motsvarar 1.0020 V, som strukten ignorerar, så `read()` returnerar fortfarande
temperaturen från `adcVal = 204`. Skillnaden är exakt ett ADC-steg, omräknat till grader.

---

## Vad som medvetet inte testas
* **Spänningar utanför mätområdet.** Att `setInputVoltage()` ignorerar t.ex. -0.1 eller 5.1 V,
  och att den tidigare spänningen då ligger kvar, testas inte. Det är ett naturligt andra
  testfall för den som vill bygga vidare, och mer om gränsvärden kommer i **L09**.
* **En riktig ADC.** Strukten tar emot spänningen direkt, och omräkningen från ADC-värde till
  spänning finns bara i testets hjälpfunktion. Det är den delen som `driver::tempsensor::Tmp36`
  i er egen kodbas behöver en `driver::adc::Stub` för.

---
