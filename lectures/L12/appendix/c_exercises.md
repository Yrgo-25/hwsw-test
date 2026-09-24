# Bilaga C - Uppgifter

Uppgifterna nedan görs i **ert eget repo** och är en del av projektarbetet i **P04**.

---

## Gränsfall för `Logic`
* Fyll i en tabell med gränsfall för `system::logic::Logic`, t.ex. hämtade från listan i bilaga A
  eller egna ni identifierar själva:

  | Gränsfall | Indata/situation | Förväntat beteende |
  |---|---|---|
  | t.ex. Ogiltig period | `"period 0"` | ... |

* Skriv komponenttester som verifierar gränsfallen i tabellen. Beter sig `Logic` inte som ni
  förväntat er, åtgärda produktionskoden och dokumentera beslutet.
* (VG) **P04** kräver minst tre gränsfall som inte redan täcks av era G-tester. Ta gärna med
  minst ett med samtidiga händelser (se bilaga B).
* Lägg gärna till ett gränsfall kopplat till EEPROM eller watchdog, om ni implementerat det.

---

## Ogiltig konfiguration eller ogiltig indata?
Gå igenom era egna klasser i **P02**/**P03** och kontrollera, enligt bilaga B, att ogiltiga
konstruktorargument hanteras med ett kontrollerat avslut, medan ogiltig indata till ett redan
skapat objekt avvisas med ett returvärde. Skriv ett test för minst ett fall där ogiltig indata
avvisas och tillståndet består.

---

## Sanitizers (VG)
Bygg om er testsvit med `-fsanitize=address,undefined -fno-sanitize-recover=all` och kör den:
* Hittade `ASan` eller `UBSan` något fel?
* Läs felrapporten och identifiera var i koden felet uppstod.
* Åtgärda felet och kör sedan testsviten igen.

---
