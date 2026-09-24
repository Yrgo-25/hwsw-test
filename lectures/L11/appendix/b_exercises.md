# Bilaga B - Övningsuppgifter

## Uppvärmning: färdigställ `logic_test.cpp`
Med stubbarna från **L08–L09** på plats går övningsbibliotekets komponenttest att bygga:
* Ta bort `#ifdef STUBS_IMPLEMENTED`/`#endif` i
  [logic/logic_test.cpp](../../../libs/atmega/test/logic/logic_test.cpp).
* Kör testsviten. Det färdiga testfallet `DebounceHandling` ska vara grönt direkt; är det inte
  det, kontrollera stubbarna mot kraven i [bilaga A i L09](../../L09/appendix/a_exercises.md)
  innan ni misstänker `logic::Logic`.
* Fyll i de återstående testfallen (`ToggleHandling`, `TempHandling`, `Eeprom`) enligt
  kommentarerna i filen, och kör om testsviten tills samtliga tester är gröna.

```bash
cd libs/atmega/test
make
```

---

## Komponenttester för er egen `Logic`
Utgå från exemplet i [bilaga A i L08](../../L08/appendix/a_component_tests.md) och skriv, i ert
eget repo, ett komponenttest som verifierar `"on"`-kommandot. Stäm därefter av i gruppen mot
[Komponenttester (G)](../../../projects/P04/README.md#komponenttester-g) i **P04** och fördela de
återstående scenarierna mellan er: `"off"`, blinkläge, `"temp"` samt `"status"`.

Stäm samtidigt av övriga G-krav i [P04](../../../projects/P04/README.md#krav). Glöm inte:
* Enhetstester för `driver::tempsensor::Smart` samt den algoritm ni valde i Fas 2 i **P03**. De
  testas med samma teknik som `Tmp36` och `ml::lin_reg::Adaptive` i **L07**: kända, framräknade
  referensvärden och `EXPECT_NEAR`.
* En testsvit per testad klass, t.ex. `TEST(Tmp36, ...)` i en egen testfil.

---

## Diskussion
* Hur löste ni problemet med att komma åt de stubb-instanser som `driver::factory::Stub` skapar
  internt åt `Logic`?
* Vad skulle hända med testets hastighet och determinism om ni använde de riktiga
  `Esp32s3`-drivers i stället för stubbar i ett komponenttest?

---
