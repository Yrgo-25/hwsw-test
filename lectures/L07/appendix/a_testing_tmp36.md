# Bilaga A - Enhetstester för `Tmp36`

## En driver med beräkningslogik
Reglerna för ett bra enhetstest från **L01** gäller lika mycket här: ett tydligt avgränsat
beteende per test, deterministiskt, snabbt, oberoende. `driver::tempsensor::Tmp36` skiljer sig
ändå från GPIO-, seriell- och timer-drivern (**L02–L04**): i stället för att verifiera
tillståndshantering (skrivet/lästa nivåer, timeout) verifierar ni att en formel räknar rätt,
vilket ställer ett par nya krav.

---

## Flyttal och toleranser
Att jämföra flyttal med `==` är i praktiken opålitligt, avrundningsfel gör att två matematiskt
identiska uttryck kan ge marginellt olika resultat i koden. Använd därför `EXPECT_NEAR(a, b, tol)`
i stället för `EXPECT_EQ(a, b)` när ni verifierar **flyttalsresultat**, och välj ett toleransvärde
som är rimligt i förhållande till vad ni faktiskt testar, t.ex. `1e-6` för en enskild beräkning.

**OBS!** Det gäller bara när returvärdet faktiskt är ett flyttal. Övningsbibliotekets
`Tmp36::read()` returnerar `int16_t`, alltså en redan avrundad temperatur, och där ska ni
använda `EXPECT_EQ` mot ett för hand avrundat värde. `EXPECT_NEAR` på två heltal är en
likhetsjämförelse förklädd till tolerans. Ni får verklig nytta av `EXPECT_NEAR` i era
ML-beräkningar längre ned, som räknar i flyttal.

---

## Testa mot ett känt, för hand uträknat exempel
Det bästa underlaget för ett enhetstest av en formel är ett exempel ni räknar för hand: en känd
inspänning, och en känd förväntad temperatur. Genom att mata in exakt samma inspänning i er kod
och jämföra utdatan mot det för hand uträknade resultatet kan ni verifiera att implementationen
faktiskt beräknar rätt, inte bara att den *kompilerar och kör*.

---

## `driver::tempsensor::Tmp36`
Formeln som räknar om en TMP36-sensors inspänning till en temperatur i grader Celsius är precis
den typ av beräkningslogik som passar för ett enhetstest. `Tmp36` tar emot sin
`driver::adc::Interface&` via konstruktorn (se **L05**), vilket betyder att den, precis som
`system::logic::Logic`, aldrig anropar ESP-IDF direkt och därför är fullt testbar redan nu, utan
mockningstekniken från **L06**.

### Formeln
En TMP36 ger 500 mV vid 0 °C och 10 mV per grad:

```text
Vut = 0.5 + 0.01 * T        =>        T(°C) = (Vut - 0.5) / 0.01 = 100 * Vut - 50
```

ADC:n ger inte en spänning direkt, utan ett heltal. Med n bitars upplösning och
referensspänningen Vref gäller:

```text
Vut = adcVärde / (2^n - 1) * Vref
```

### Räkneexempel
Med en 10-bitars ADC (`2^10 - 1 = 1023`), `Vref = 5.0 V` och `adcVärde = 154`:

```text
Vut = 154 / 1023 * 5.0 = 0.7527 V
T   = 100 * 0.7527 - 50 = 25.27 °C   ->  avrundat: 25 °C
```

Det är precis så [tmp36_test.cpp](../../../libs/atmega/test/driver/tempsensor/tmp36_test.cpp) i
övningsbiblioteket räknar, via hjälpfunktionerna `computeInputVoltage()` och `convertToTemp()`.

**OBS!** Siffrorna ovan gäller övningsbibliotekets ATmega328p-uppsättning. Er egen `ESP32-S3`
har 12 bitars ADC (`2^12 - 1 = 4095`) och en annan referensspänning, så samtliga mellanled blir
andra. Räkna ut ert eget exempel utifrån den upplösning och Vref som er `driver::adc::Esp32s3`
faktiskt använder, i stället för att återanvända talen här.

Genom att skicka in en `driver::adc::Stub` med ett känt, förbestämt ADC-värde (`setValue()`),
som motsvarar en känd inspänning, kan ni verifiera att `Tmp36` räknar ut exakt den temperatur
formeln säger.

---

## Samma teknik gäller era ML-algoritmer
**P04** kräver även enhetstester för `ml::lin_reg::Adaptive` samt minst ett neuralt
nätverkslager från **P03** (se projektbeskrivningen). Det är samma teknik som ovan, tillämpad
på en annan beräkning: mata in ett känt, för hand uträknat exempel (indata, parametrar och
förväntat resultat) och verifiera utdatan med `EXPECT_NEAR`. Ingen ny teknik introduceras här,
bara samma princip applicerad på er egen kodbas från **P03**.

---
