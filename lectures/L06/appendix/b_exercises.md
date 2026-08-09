# Bilaga B - Övningsuppgifter
Arbeta i grupp, i er egen kodbas från **P02**. Ni får ingen fullständig genomgång av lösningen
innan ni börjar, testa er fram utifrån principen i bilaga A och fråga vid behov.

Mocka er `driver::gpio::Esp32s3` i sin helhet:
* Gå igenom drivern och lista samtliga ESP-IDF-funktioner den anropar, inte bara
  `gpio_set_level()`/`gpio_get_level()` som i bilaga A.
* Skriv en egen header och källkodsfil (t.ex. `test/esp/gpio_mock.h`/`.c`), med samma mönster
  som i bilaga A, som innehåller en implementation av varje funktion ni listade.
* Uppdatera testsvitens byggsystem så att den länkar mot er mock i stället för mot ESP-IDF:s
  riktiga gpio-komponent.
* Skriv enhetstester för `driver::gpio::Esp32s3` mot mocken, t.ex. att rätt pin och nivå skickas
  vid `write()`, samt att drivern initieras korrekt.

---

## Diskussion
* Varför behöver driverklassens egen kod inte ändras alls för att kunna testas mot en mockad
  ESP-IDF-funktion?
* Vad är skillnaden mellan att mocka en ESP-IDF-funktion (den här övningen) och att skapa en
  C++-stubbklass som ärver `driver::gpio::Interface` (som i **L01**)? När används respektive
  teknik, och vilken av dem används när ni **komponenttestar** systemlogiken i **L08**?

---
