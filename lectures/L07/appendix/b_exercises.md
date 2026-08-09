# Bilaga B - Övningsuppgifter

Skriv enhetstester för er egen `driver::tempsensor::Tmp36` från **P02**, med en
`driver::adc::Stub` som ger ett känt, förbestämt ADC-värde:
* Räkna för hand vilken temperatur ett givet ADC-värde ska motsvara, med er egen ADC:s
  upplösning och referensspänning, och verifiera att `Tmp36` returnerar samma resultat. Se
  räkneexemplet i [bilaga A](./a_testing_tmp36.md#räkneexempel).
* Välj rätt makro: `EXPECT_EQ` om er `read()` returnerar ett heltal, `EXPECT_NEAR` med en
  motiverad tolerans om den returnerar ett flyttal.
* Testa minst ett gränsfall, t.ex. ett ADC-värde i utkanten av mätområdet (0 respektive
  `maxValue()`). Mer om systematisk gränsvärdesanalys i **L09**.

---

## ML-algoritmerna från P03
**P04** kräver enhetstester även för era ML-algoritmer, och det är samma teknik tillämpad på en
annan beräkning (se [bilaga A](./a_testing_tmp36.md#samma-teknik-gäller-era-ml-algoritmer)):
* Skriv ett enhetstest för `ml::lin_reg::Adaptive` som verifierar en träningsiteration mot ett
  för hand uträknat exempel: kända startvikter, känd indata och lärhastighet, framräknat
  förväntat resultat. Här returneras flyttal, så `EXPECT_NEAR` är rätt makro.
* Verifiera minst ett gränsfall, t.ex. ogiltig indata eller en ogiltig lärhastighet.
* Om ni hinner: gör motsvarande för ett av era neurala nätverkslager (t.ex. `Dense`).

---

## Fler riktiga drivers, på egen hand
Ni har nu sett mockningstekniken från **L06** tillämpad på `driver::gpio::Esp32s3`. Använd
resterande tid till att skriva enhetstester för minst en till av era riktiga `Esp32s3`-drivers,
t.ex. `adc`, eller `eeprom`/`watchdog` om er grupp implementerade dem i **P02**. Ingen genomgång
ges för detta, arbeta i gruppen precis som i **L06**.

---

## Diskussion
* Varför är `EXPECT_EQ` olämpligt för flyttalsjämförelser, även när beräkningen "borde" ge ett
  exakt värde?
* `Tmp36` ligger i driverlagret men behöver ändå ingen ESP-IDF-mockning för att enhetstestas.
  Vad är det i dess design (se **L05**) som gör det möjligt?
* Vilken/vilka drivers testade ni utöver `gpio`, och vad behövde ni mocka för respektive?

---
