# Bilaga B - Övningsuppgifter

Uppgifterna nedan görs i grupp och i **ert eget repo**, och utgör första delen av G-kravet
[Mätdatainsamling och automatiserade integrationstester](../../../projects/P04/README.md#mätdatainsamling-och-automatiserade-integrationstester-g)
i **P04**. De gås inte igenom i helklass: varje grupps firmware och protokoll ser olika ut, så det
finns ingen gemensam lösning att visa. Läraren går i stället runt och hjälper till.

---

## Protokoll
Bestäm vilken mätdata ni ska samla in, t.ex. temperaturprediktioner över tid eller tidmätning
av blinktogglingens precision:
* Bestäm om insamlingen ska ske fråga-svar eller strömmande (se [bilaga A](./a_measurement_data.md)).
* Ange exakt vilka fält varje rad ska innehålla, i vilken ordning och med vilken enhet.
* Skriv tre exempelrader enligt det valda formatet.
* Dokumentera protokollet i ert repo, t.ex. i `README.md`.

---

## Firmware
* Krävs ett nytt kommando, t.ex. `"log on"`/`"log off"`, så implementera det i
  `system::logic::Logic`, och skriv komponenttester för det, precis som för de befintliga
  kommandona.
* Flasha kortet och kontrollera i `idf.py monitor` att raderna kommer ut i rätt format.

---

## Insamling och lagring
* Skriv ett Python-skript som läser mätdatan från den seriella porten och sparar den som CSV med
  kolumnrubriker. Som utgångspunkt finns ett tomt skal i
  [libs/atmega/test/scripts/serial_test.py](../../../libs/atmega/test/scripts/serial_test.py);
  kopiera det till ert eget repo, t.ex. till `scripts/`, och bygg vidare därifrån.
* Ta emot portnamn, och gärna mättid, som argument till skriptet.
* Bestäm och implementera hur skriptet hanterar trasiga eller ofullständiga rader, och skriv ut
  hur många rader som kasserades när insamlingen är klar.
* Gör en första mätning på minst ett par minuter och checka in CSV-filen, så att ni har riktig
  data att arbeta med i **L17**.

Skriptet behöver `pyserial`, och från **L17** även `matplotlib`. Lägg till dem i en
`requirements.txt` i ert eget repo (kursrepots [requirements.txt](../../../requirements.txt) kan
användas som mall) och installera dem:

```bash
pip install -r requirements.txt
```

---

## Diskussion
* Vad händer i ert skript om kortet startas om mitt i en mätning? Märks det i CSV-filen?
* Vilka förutsättningar behöver dokumenteras för att någon annan ska kunna upprepa er mätning?

---
