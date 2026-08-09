# Bilaga A - Komponenttester

## Vad är ett komponenttest?
Ett **komponenttest** verifierar hur flera klasser samverkar, till skillnad från ett enhetstest
som verifierar en enda klass i isolation. I ert fall är `system::logic::Logic` själva
testobjektet: den enda klass som ska vara **riktig** i testet. Allt den beror på, de sex
drivers i **P02** (adc, factory, gpio, serial, tempsensor, timer), ersätts med stubbar som ni
redan byggde i **P02** och skriver enhetstester för som en del av **P04**.

### Var går gränsen mot ett enhetstest?
Ett komponenttest med bara en riktig klass ser vid första anblick ut som ett enhetstest, men
skillnaden ligger i **vad som verifieras**:
* Ett **enhetstest** verifierar en enskild klass egna returvärden och tillståndsövergångar.
* Ett **komponenttest** verifierar ett helt *flöde* genom flera samverkande enheter: kommando in
  via serial → tolkning i `Logic` → timern startas → LED-tillståndet ut. Stubbarna utgör bara
  systemets ytterkant, där flödet börjar och slutar.

Det är den distinktionen ni ska kunna motivera i **P04**s redovisning, och den återkommer på det
praktiska provet.

### Testets uppbyggnad
Notera att `Logic` bara får *en* sak injicerad, factoryn. Övriga fem stubbar skapas av factoryn,
vilket är precis det som gör åtkomstfrågan nedan intressant:

```text
                   system::logic::Logic          <- enda RIKTIGA klassen (testobjektet)
                             |
                             | injiceras i konstruktorn
                             v
                   driver::factory::Stub         <- STUBB
                             |
                             | skapar
        +----------+---------+---------+----------+
        v          v         v         v          v
   adc::Stub  gpio::Stub serial::Stub  |     timer::Stub      <- STUBBAR
                                  tempsensor::Stub
        ^          ^         ^         ^          ^
        |          |         |         |          |
        +----------+---------+---------+----------+
                             |
             testet måste nå dessa instanser för att kunna
             mata in indata och läsa av resultatet
```

---

## Ett exempel, i ord
`Logic` tar emot en `driver::factory::Interface&` i sin konstruktor och skapar sina drivers
genom den (se **L05**). Ett komponenttest av `Logic` skulle därför:
1. Skapa en `driver::factory::Stub` och skicka in den till `Logic`s konstruktor.
2. Mata in ett serial-kommando, t.ex. `"on"`, i den `driver::serial::Stub` som factory-stubben
   skapade.
3. Köra `Logic` ett steg (via `run()`, med en stoppvillkor som avbryter loopen efter en
   iteration).
4. Verifiera att `driver::gpio::Stub` nu speglar att LED:en är tänd.

För att steg 2 och 4 ska fungera behöver testet komma åt de *specifika* stubb-instanser som
`driver::factory::Stub` skapade åt `Logic`, inte bara veta att de finns. Det är ett verkligt
designbeslut ni behöver ta ställning till: hur exponerar er `driver::factory::Stub` de skapade
instanserna, så att testet både kan styra dem (mata in kommandon, trigga timeout) och läsa av
dem (kontrollera LED-tillstånd, utskriven text) i efterhand?

---

## Fler scenarier värda att komponenttesta
* **Blinkläge:** `"blink on"` följt av att `driver::timer::Stub`s timeout triggas ska växla
  `driver::gpio::Stub`s tillstånd, och `"blink off"` ska släcka LED:en och inaktivera
  toggling.
* **Temperatur:** `"temp"` ska läsa av `driver::tempsensor::Stub`s simulerade temperatur och
  skriva ut den via `driver::serial::Stub`.
* **Status:** `"status"` ska skriva ut blinkläge, period och aktuell temperatur, allt hämtat via
  stubbarna, inte hårdkodat.

---

## Vad ska vara riktigt, vad ska stubbas?
* **Riktigt:** `system::logic::Logic`, vars *eget* beteende (kommandotolkning, blinklogik,
  statushantering) testet ska verifiera.
* **Stubbat:** samtliga sex drivers, som har egna enhetstester i **P04**, och som skulle göra
  testet långsamt eller icke-deterministiskt om de vore riktiga.

---
