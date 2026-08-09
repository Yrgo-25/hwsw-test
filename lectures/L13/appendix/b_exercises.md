# Bilaga B - Övningsuppgifter
Studera `.github/workflows/ci.yml` i ert eget repo (workflowen ni kopierade in i **L05**) och beskriv hur pipelinen är uppbyggd:
* Vad utlöser pipelinen (`on:`)?
* Vilka jobb (`jobs:`) innehåller den, och vilket syfte har respektive jobb?
* Vilka steg använder `uses:` respektive `run:`?
* Hur används den `.clang-format`-fil och de formatteringsskript som ni lade till i **L05**?

Anpassa därefter pipelinen så att den:
* bygger och kör er testsvit automatiskt;
* bygger er firmware via ESP-IDF som ett eget jobb;
* kontrollerar kodformattering med `clang-format`, samt era Python-skript med `black`.

Verifiera slutligen att pipelinen:
* Blir godkänd när samtliga jobb lyckas.
* Blir misslyckad om ett test, firmware-bygget eller formatteringskontrollen misslyckas.

---

## Diskussion
* Vad händer i er pipeline om en gruppmedlem pushar kod som får ett test att misslyckas? Var syns det, och för vem?
* Varför körs `build-and-test` och `format-check` som två separata jobb i stället för ett enda jobb med alla steg i följd?
* Varför räcker det att bygga firmwaren i CI, utan att flasha och köra den på en riktig `ESP32-S3`, för att fånga många relevanta fel?

---
