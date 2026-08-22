# Bilaga B - Övningsuppgifter

Övningarna nedan görs i **ert eget repo** och är en del av projektarbetet i **P04**. De gås inte
igenom i helklass efteråt, eftersom varje grupps kodbas ser olika ut; utgå i stället från demot
på genomgången och fråga under lektionen när ni kör fast.

---

## Läs er egen pipeline
Studera `.github/workflows/ci.yml` i ert eget repo (workflowen ni satte upp i **L05**, och
byggde ut i **L06** och sannolikt även i **L12**) och beskriv hur pipelinen är uppbyggd:
* Vad utlöser pipelinen (`on:`)?
* Vilka jobb (`jobs:`) innehåller den, och vilken fråga besvarar respektive jobb?
* Vilka steg använder `uses:` respektive `run:`?
* Hur används den `.clang-format`-fil som ni lade till i **L05**?
* Vad skulle sluta fungera om ni tog bort `actions/checkout@v4` ur ett jobb?

---

## Se över ordningen mellan jobben
Er pipeline har jobb för formattering och firmware-bygge sedan **L05**, och för testkörning sedan
**L06**. Nu när testsviten dessutom vuxit rejält är det värt att se över hur de förhåller sig till
varandra:
* Rita upp den nuvarande kedjan. Vilka jobb väntar på vilka, via `needs:`?
* Bestäm var testjobbet hör hemma. Ska det köras parallellt med firmware-bygget, före det, eller
  ska firmware-bygget bero på att testerna går igenom? Motivera utifrån hur lång tid jobben tar
  och vilket besked ni helst vill ha först.
* Mät hur lång tid varje jobb faktiskt tar (syns per jobb under *Actions*), och använd siffrorna
  i motiveringen i stället för magkänsla.

---

## Härda pipelinen
* **Branch protection:** konfigurera `main` så att en pull request inte går att merga förrän
  pipelinens jobb har lyckats (*Settings → Branches*, *Require status checks to pass*). Testa
  genom att öppna en pull request med ett medvetet trasigt test och kontrollera att
  merge-knappen faktiskt blockeras.
* **`concurrency`:** lägg till avbrytning av överflödiga körningar enligt bilaga A. Pusha två
  gånger snabbt i följd och kontrollera att den första körningen avbryts.
* **Caching:** identifiera det dyraste steget i er pipeline (sannolikt ESP-IDF-bygget) och
  resonera kring vad som skulle gå att cacha, och vad cachen i så fall bör brytas mot.
* **Artefakter:** kontrollera vilken `retention-days` ert firmware-jobb använder. Är den rimlig
  för hur ni faktiskt använder artefakterna?

Verifiera slutligen att pipelinen:
* Blir godkänd när samtliga jobb lyckas.
* Blir misslyckad om ett test, firmware-bygget eller formatteringskontrollen misslyckas.

---

## Diskussion
* Vad händer i er pipeline om en gruppmedlem pushar kod som får ett test att misslyckas? Var syns
  det, och för vem?
* Varför körs det här repots `build-and-test` och `format-check` som två separata jobb i stället
  för ett enda jobb med alla steg i följd?
* Varför räcker det att bygga firmwaren i CI, utan att flasha och köra den på en riktig
  `ESP32-S3`, för att fånga många relevanta fel?
* Vilket av era jobb är mest värdefullt i förhållande till hur lång tid det tar att köra? Vilket
  är minst värdefullt?

---
