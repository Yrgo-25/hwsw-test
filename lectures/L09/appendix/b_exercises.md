# Bilaga B - Övningsuppgifter

## Identifiera gränsfall
Utgå från komponenttesterna ni påbörjade i **L08**, samt exemplen i bilaga A. Fyll i en tabell
med minst tre gränsfall för `system::logic::Logic`, t.ex. hämtade från listan i bilaga A eller
egna ni identifierar själva:

| Gränsfall | Indata/situation | Förväntat beteende |
|---|---|---|
| t.ex. Ogiltig period | `"period 0"` | ... |

(VG) Lägg till minst ett gränsfall kopplat till EEPROM eller watchdog, om ni implementerat det.

---

## Skriv komponenttester
Skriv komponenttester som verifierar samtliga gränsfall i er tabell, i er egen kodbas från
**P02**, som en fortsättning på komponenttesterna ni påbörjade i **L08**.

**OBS!** Gränsfallen ovan handlar om kommandotolkning, och finns bara i er egen
`system::logic::Logic`. Övningsbibliotekets `logic::Logic` (utan `system::`) tolkar inga
kommandon alls, utan har bara `run()` samt sina `handle*()`-metoder, så
[logic/logic_test.cpp](../../../libs/atmega/test/logic/logic_test.cpp) är inte rätt fil för den
här uppgiften. Vill ni ändå öva gränsvärdesanalys i övningsbiblioteket finns andra gränser att
utgå från: en timer med 0 ms timeout, ett ogiltigt pin-id, en EEPROM-adress i utkanten av
adressrymden, eller ett andra knapptryck inom debounce-fönstret.

---

## Diskussion
* Varför räcker det sällan att bara testa "typiska" indata i ett system som styr fysisk
  hårdvara?
* Hittade ni något gränsfall där er nuvarande implementation inte gjorde vad ni förväntade er?
  Vad var det, och hur allvarligt hade det varit i en produkt hos en verklig kund?

---
