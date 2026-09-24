# Bilaga B - Uppgifter

Uppgifterna nedan görs i **ert eget repo** och utgör VG-kravet
[Mätdatainsamling och HW/SW-integration](../../../projects/P04/README.md#mätdatainsamling-och-hwsw-integration-vg)
i **P04**, med undantag för det manuella integrationstestet, som tas upp i **L17**.

---

## Protokoll
Bestäm vilken mätdata ni ska samla in, t.ex. temperaturprediktioner över tid eller tidmätning
av blinktogglingens precision:
* Ange exakt vilka fält varje rad ska innehålla, i vilken ordning och med vilken enhet.
* Skriv tre exempelrader enligt det valda formatet.

---

## Insamling och lagring
* Skriv ett Python-skript som läser mätdatan från den seriella porten och sparar den som CSV med
  kolumnrubriker. Som utgångspunkt finns ett tomt skal i
  [libs/atmega/test/scripts/serial_test.py](../../../libs/atmega/test/scripts/serial_test.py);
  kopiera det till ert eget repo och bygg vidare därifrån.
* Bestäm och implementera hur skriptet hanterar trasiga eller ofullständiga rader.

Skriptet behöver `pyserial` och `matplotlib`. Lägg till dem i en `requirements.txt` i ert eget
repo (kursrepots [requirements.txt](../../../requirements.txt) kan användas som mall) och
installera dem:

```bash
pip install -r requirements.txt
```

---

## Visualisering
* Rita upp mätdatan med `matplotlib`, med kravet inritat i samma graf.
* Avgör utifrån grafen om systemet uppfyller kravet, och motivera bedömningen.

---
