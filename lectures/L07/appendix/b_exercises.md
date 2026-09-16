# Bilaga B - Övningsuppgifter

## Skriv och testa en `Tmp36`-strukt
I filen `driver/tmp36.h`, implementera en strukt `driver::Tmp36`, som håller en inspänning och
räknar om den till en temperatur. Lägg till följande:
* `#pragma once` högst upp i filen.
* En privat medlemsvariabel för inspänningen i V, initierad till `0.0`.
* `setInputVoltage(double voltage)`: sparar spänningen, men gör ingenting om värdet ligger
  utanför mätområdet [0.0, 5.0] V.
* `read()`: returnerar temperaturen i grader Celsius, beräknad enligt databladets form av
  formeln: `T = (Uin - 0.5) / 0.01`.

Skriv nu ett testfall i [testsuite.cpp](../exercises/testsuite.cpp) som går igenom samtliga
ADC-värden i mätområdet (0–1023):
* Räkna ut inspänningen med `computeInputVoltage()` och mata in den med `setInputVoltage()`.
* Räkna ut den förväntade temperaturen med `convertToTemp()`.
* Jämför den mot `read()`.

Jämför nu förväntad och faktisk temperatur för varje ADC-värde:
* Börja med att använda `EXPECT_EQ()`. Hur blir resultatet och varför?
* Testa sedan att använda `EXPECT_NEAR()` med en viss tolerans, exempelvis `1e-2`. Hur blir
  resultatet nu?
* Vilken tolerans är rimlig här, och vad hade `1e-2` kunnat dölja?

---
