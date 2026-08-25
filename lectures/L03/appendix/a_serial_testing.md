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
Testfilen simulerar därför hårdvarusidan i egna trådar, som pollar `UDR0`/`UCSR0A` och svarar
ungefär som den riktiga hårdvaran skulle göra, medan drivern anropas från en annan tråd
(`Transmit`) respektive från huvudtråden (`Read`). Ni har redan använt `std::thread` i en
tidigare kurs; själva testtekniken är ny, inte verktyget.

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

sätter stop = true       avslutar loopen              avslutar loopen
```

Två saker är värda att notera:
* **`printThread()` äger stoppvillkoret.** Det är den som sätter `stop` när hela strängen
  skickats, och det är enda anledningen till att de andra två trådarna avslutar sina loopar.
  Lämnar ni den delen oimplementerad blir testet varken grönt eller rött: de två andra trådarna
  snurrar vidare och testsviten hänger, tills ni avbryter den med Ctrl+C.
* **En `EXPECT_*` som misslyckas i en tråd kastar ett undantag.** Ett undantag som tar sig ut ur
  en `std::thread` avslutar hela testprogrammet via `std::terminate`, oavsett om trådfunktionen
  är `noexcept` eller inte. Ni får alltså ingen röd rad, utan en död testsvit. Lägg därför era
  kontroller där ett fel faktiskt går att rapportera: på huvudtråden, efter `join()`, som i
  `Read`-testet.

## Så läses data in
`Read`-testet vänder på rollerna. Där `Transmit` låter en tråd anropa drivern, är det här
huvudtråden som anropar `read()`, medan tråden `injectRxData()` spelar hårdvara: den lägger ett
tecken i taget i `UDR0` och sätter `RXC0` ("ett tecken har tagits emot"). Samma sak gäller som på
sändarsidan - det riktig hårdvara gör av sig självt måste testet göra för hand. En läsning av
`UDR0` rensar inte `RXC0` här, så testet rensar flaggan själv, vilket samtidigt är kvittensen som
släpper fram nästa tecken:

```text
huvudtråden (testet)                 injectRxData
--------------------                 ------------
                                     UDR0 = 'H'
                                     set(UCSR0A, RXC0)
                                       = "tecken mottaget"
serial.read(&byte, 1, 100)
  -> ser att RXC0 är satt
  -> läser 'H' ur UDR0
  -> returnerar 1
clear(UCSR0A, RXC0)
  = "registret avläst"
                                     ser att RXC0 rensats
                                     UDR0 = 'i'
                                     set(UCSR0A, RXC0)
          ...                                  ...

t1.join()                            loopen slut när hela
EXPECT_EQ(bytesRead, ...)            strängen matats in
```

Fyra saker är värda att notera:
* **Samtliga kontroller ligger på huvudtråden, efter `join()`.** `injectRxData()` innehåller inte
  en enda `EXPECT_*`, den matar bara in tecken. Det är just därför ett fel på mottagarsidan
  rapporteras som ett vanligt rött testfall, och inte som en död testsvit enligt punkten ovan.
* **Väntan på kvittensen är tidsbegränsad.** `injectRxData()` väntar som mest 1000 µs på att
  `RXC0` ska rensas innan den går vidare till nästa tecken. Glömmer ni att rensa flaggan efter en
  läsning blir testet därför rött - drivern läser då samma tecken om och om igen - i stället för
  att hänga.
* **Felfallen testas innan tråden startas.** `read()` ska returnera -1 för en nullpekare och för
  storleken 0, samt 0 när inget tecken kommit inom tidsgränsen. Rensa `RXC0` före det sista
  fallet, annars läser drivern det som råkar ligga kvar i `UDR0` sedan `Transmit`-testet. Notera
  att fallet kostar sina fulla 100 ms, eftersom `read()` pollar `RXC0` en gång per millisekund.
* **Använd inte `timeout_ms = 0` i ett enhetstest.** Drivern väntar då utan tidsgräns på `RXC0`,
  tills bufferten är full. Ett sådant testfall hänger testsviten så fort antalet inmatade tecken
  inte råkar stämma exakt.

---
