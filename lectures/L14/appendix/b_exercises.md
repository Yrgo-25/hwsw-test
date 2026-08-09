# Bilaga B - Övningsuppgifter
Beskriv den mätdata ni planerar att samla in i **P04**, till exempel temperaturprediktioner över tid eller tidsmätning av en drivers beteende:
* Ange exakt vilka fält varje rad ska innehålla, i vilken ordning och med vilken enhet.
* Skriv tre exempelrader enligt det valda formatet.
* Beskriv hur Python-skriptet ska hantera en rad som är trasig eller ofullständig, till exempel
  på grund av brus på linjen.
* Kontrollera att ett enkelt Python-skript kan läsa och tolka era exempelrader. Som utgångspunkt
  finns ett tomt skal i
  [libs/atmega/test/scripts/serial_test.py](../../../libs/atmega/test/scripts/serial_test.py),
  med en beskrivning av vad skriptet ska göra. Kopiera det till ert eget repo och bygg vidare
  därifrån.

`pyserial` installeras via repots `requirements.txt`:

```bash
pip install -r requirements.txt
```

---

## Diskussion
* Vad händer om `ESP32`-sidan och Python-sidan är oense om formatet, till exempel om den ena
  sidan använder punkt som decimaltecken medan den andra använder komma, trots att komma även
  används som fältavgränsare?
* Varför är `readline()` tillsammans med en avslutande radbrytning ett enkelt sätt att avgöra
  var en mätning slutar och nästa börjar?

---
