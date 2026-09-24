# Bilaga A - Enhetstester för `Tmp36`

## En driver med beräkningslogik
Reglerna för ett bra enhetstest från **L01** gäller lika mycket här:
* Ett tydligt avgränsat beteende per test.
* Deterministiskt.
* Snabbt.
* Oberoende.

Skillnaden mot GPIO-, seriell- och timer-drivern (**L02–L04**) ligger i vad ni verifierar: i
stället för tillståndshantering (skrivna/lästa nivåer, timeout) verifierar ni att en formel
räknar rätt. Det ställer ett par nya krav.

---

## Flyttal och toleranser
Att jämföra flyttal med `==` är i praktiken opålitligt, avrundningsfel gör att två matematiskt
identiska uttryck kan ge marginellt olika resultat i koden. Av denna anledning:
* Använd `EXPECT_NEAR(a, b, tol)` i stället för `EXPECT_EQ(a, b)` när ni verifierar flyttal.
* Välj ett toleransvärde som är rimligt i förhållande till vad ni faktiskt testar, t.ex. `1e-6`
  för en enskild beräkning.

Temperaturen i [bilaga B](./b_exercises.md) är precis ett sådant fall: den räknas ut på två
matematiskt identiska sätt, `T = (Uin - 0.5) / 0.01` och `T = 100 * Uin - 50`, och `EXPECT_EQ`
blir ändå rött.

**OBS!** Det gäller bara när returvärdet faktiskt är ett flyttal. Övningsbibliotekets
`Tmp36::read()` returnerar `int16_t`, alltså en redan avrundad temperatur, och där ska ni
använda `EXPECT_EQ` mot ett för hand avrundat värde. `EXPECT_NEAR` på två heltal är en
likhetsjämförelse förklädd till tolerans. Det är alltså returtypen som avgör makrot, inte vad
som mäts.

---

## Formeln
En TMP36 ger 500 mV vid 0 °C och 10 mV per grad:

```text
Uin = 0.5 + 0.01 * T        =>        T(°C) = (Uin - 0.5) / 0.01 = 100 * Uin - 50
```

ADC:n ger inte en spänning direkt, utan ett heltal. Med n bitars upplösning och
referensspänningen Uref gäller:

```text
Uin = adcVärde / (2^n - 1) * Uref
```

`computeInputVoltage()` i [testsuite.cpp](../exercises/testsuite.cpp) räknar med en 10-bitars ADC
och `Uref = 5.0 V`. Övningsbibliotekets
[tmp36_test.cpp](../../../libs/atmega/test/driver/tempsensor/tmp36_test.cpp) räknar likadant, men
avrundar resultatet till `int16_t` och jämför därför med `EXPECT_EQ`.

---

## Samma teknik gäller era ML-algoritmer
**P04** kräver enhetstester även för era ML-algoritmer från **P03**: `ml::lin_reg::Adaptive`,
`driver::tempsensor::Smart` samt den algoritm ni valde i Fas 2. Det är samma princip tillämpad på en annan beräkning: mata in ett känt, för hand
uträknat exempel (indata, parametrar och förväntat resultat) och verifiera utdatan med
`EXPECT_NEAR`.

---
