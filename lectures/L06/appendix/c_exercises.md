# Bilaga C - Övningsuppgifter

## Tankeexperiment: `Logic` utan dependency injection
Eftersom **P02** redan krävde den här typen av design finns det sannolikt inget att refaktorera
i er egen kodbas. Resonera i stället kring motsatsen: anta att `system::logic::Logic` i stället
hade sett ut så här:

```cpp
class Logic
{
public:
    Logic() : myGpio{driver::gpio::Esp32s3{}} {}
private:
    driver::gpio::Esp32s3 myGpio;
};
```

* Vad hade behövt ändras i konstruktorn och medlemsvariablerna för att göra klassen testbar
  igen, utan att ändra *vad* klassen gör?
* Skulle det räcka att byta ut `driver::gpio::Esp32s3` mot `driver::gpio::Interface&`, eller
  skulle hela factory-mönstret (som er `system::logic::Logic` faktiskt använder) behövas för att
  hantera alla klassens drivers på ett enhetligt sätt? Jämför gärna med övningsbibliotekets
  `logic::Logic`, som löser samma problem utan factory, genom att ta emot tio enskilda
  interface-referenser i konstruktorn.

---

## Diskussion: testbar design
* Varför räcker det med ett interface och en referens för att göra `Tmp36` testbar, medan
  `Logic` (som beror på flera olika drivers) i stället tar emot en hel factory?
* Vilken skillnad hade det gjort för testbarheten om `main` hade skapat drivers och skickat in
  dem en och en till `Logic`, i stället för att skicka in en factory?

---

## Kodövning: mocka `driver::gpio::Esp32s3`
Arbeta i grupp, i er egen kodbas från **P02**. Ni får ingen fullständig genomgång av lösningen
innan ni börjar, testa er fram utifrån principen i bilaga B och fråga vid behov.

Mocka er `driver::gpio::Esp32s3` i sin helhet:
* Gå igenom drivern och lista samtliga ESP-IDF-funktioner den anropar, inte bara
  `gpio_set_level()`/`gpio_get_level()` som i bilaga B.
* Skriv en egen header och källkodsfil (t.ex. `test/esp/gpio_mock.h`/`.c`), med samma mönster
  som i bilaga B, som innehåller en implementation av varje funktion ni listade.
* Uppdatera testsvitens byggsystem så att den länkar mot er mock i stället för mot ESP-IDF:s
  riktiga gpio-komponent.
* Skriv enhetstester för `driver::gpio::Esp32s3` mot mocken, t.ex. att rätt pin och nivå skickas
  vid `write()`, samt att drivern initieras korrekt.

---

## Kodövning: låt era egna tester köras automatiskt
Nu finns det äntligen egna tester att köra, så pipelinen från **L05** ska få sitt testjobb:
* Kontrollera att er testsvit byggs och körs med ett enda kommando lokalt (t.ex. `make test`),
  och att den innehåller drivertesterna ni just skrev.
* Lägg till ett `unit-tests`-jobb i `.github/workflows/ci.yml`:

  ```yaml
    unit-tests:
      name: Build and run unit tests
      runs-on: ubuntu-latest
      steps:
        - name: Checkout repository
          uses: actions/checkout@v4
          with:
            submodules: recursive
        - name: Build and run test suite
          run: make test
  ```

* Låt ett testfall misslyckas medvetet (t.ex. `EXPECT_EQ(1, 2)`), pusha, och verifiera att
  pipelinen blir röd och att felet syns i jobbets logg. Återställ därefter.
* Fundera på var jobbet hör hemma i kedjan: parallellt med firmware-bygget, eller före det via
  `needs:`? Ni behöver inte bestämma er slutgiltigt nu, frågan tas upp igen i **L13**.

---

## Diskussion
* Varför behöver driverklassens egen kod inte ändras alls för att kunna testas mot en mockad
  ESP-IDF-funktion?
* Vad är skillnaden mellan att mocka en ESP-IDF-funktion (den här övningen) och att skapa en
  C++-stubbklass som ärver `driver::gpio::Interface` (som i **L01**)? När används respektive
  teknik, och vilken av dem används när ni **komponenttestar** systemlogiken i **L08**?

---
