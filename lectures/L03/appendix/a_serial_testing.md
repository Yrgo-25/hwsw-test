# Bilaga A - Enhetstester för en seriell driver

`driver::serial::Interface` har bland annat operationerna `printf(format, ...)` för att skriva
ut formaterad text, samt `read(buffer, size, timeout_ms)` för att läsa in data, ingen av dem
använder `std::string`. Den riktiga implementationen i det delade
[ATmega328p-övningsbiblioteket](../../../libs/atmega/README.md), `driver::serial::Atmega328p`,
skriver och läser hårdvaruregister direkt (t.ex. `UDR0`, `UCSR0A`). Precis som med GPIO-drivern
i **L02** går detta att enhetstesta redan nu, via samma virtuella hårdvaruplattform, helt utan
fysisk hårdvara.

Till skillnad från GPIO- och timer-drivern sker seriell överföring byte för byte över tid: äkta
hårdvara markerar `UDR0` som redo för nästa byte (flaggan `UDRE0` i `UCSR0A`) först när den
föregående byten flyttats vidare till skiftregistret. Att överföringen är *helt* klar signaleras
separat, av `TXC0`.
Testfilen simulerar därför hårdvarusidan i en egen `std::thread`, som pollar `UDR0`/`UCSR0A` och
svarar ungefär som den riktiga hårdvaran skulle göra, samtidigt som drivern testas från en annan
tråd. Ni har redan använt `std::thread` i en tidigare kurs; själva testtekniken är ny, inte
verktyget.

## Så samverkar trådarna
`Transmit`-testet startar tre trådar som kommunicerar enbart via de virtuella registren `UDR0`
och `UCSR0A`, precis som drivern och hårdvaran gör på riktigt:

```text
printThread              simulateDataReg              readDataRegThread
(drivern)                ("hårdvaran")                (mottagaren)
-----------              ---------------              -----------------
serial.printf("Hi")
  -> skriver 'H'
     till UDR0
                         ser att UDR0 ändrats
                         väntar TransmissionDelay_us
                         clear(UCSR0A, UDRE0)
                           = "buffern upptagen"
                                                      ser att UDRE0 är rensad
                                                      läser 'H' ur UDR0
                                                      EXPECT_EQ(byte, 'H')
                                                      set(UCSR0A, UDRE0)
                                                        = "redo för nästa"
  -> skriver 'i'
     till UDR0                    ...                          ...

sätter stopFlag = true   avslutar loopen              avslutar loopen
```

Två saker är värda att notera:
* **`printThread()` äger stoppvillkoret.** Det är den som sätter `stopFlag` när hela strängen
  skickats, och det är därför de andra två trådarna kan avsluta. Skriver ni inte den delen får
  ni ett rött test, inte ett hängande, tack vare den tidsgräns (`TestTimeout_ms`) som väntelooparna
  har.
* **En `EXPECT_*` som misslyckas i en tråd kastar ett undantag.** Ett undantag som tar sig ut ur
  en `std::thread` avslutar hela testprogrammet direkt. Testet fångar därför undantaget i tråden
  och kastar om det på huvudtråden efter `join()`, så att felet rapporteras som ett vanligt rött
  testfall. Använd samma mönster om ni lägger till egna trådade tester.

---
