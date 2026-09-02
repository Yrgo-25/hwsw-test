# Bilaga A - Enhetstester för timer-drivern
`driver::timer::Interface` har bland annat operationerna `start()`/`stop()`,
`setTimeout_ms(timeout_ms)` samt `hasTimedOut()`.

Den riktiga implementationen `driver::timer::Atmega328p` tar dessutom emot en valfri callback som konstruktorargument för att köras vid timeout. Denna callback utgör alltså inte en del av `Interface`, utan är specifik för den konkreta implementationen.

`Atmega328p` skriver och läser hårdvaruregister direkt (t.ex. `TCCR0A`, `TIMSK0`). Precis som
med GPIO- och serial-drivern går detta att enhetstesta redan nu, via samma virtuella
hårdvaruplattform, helt utan fysisk hårdvara och utan att vänta in riktig tid.

---

## Varför en timer är extra tacksam att testa virtuellt
En timer mäter tid, och tid är det värsta man kan ha i ett enhetstest: ett test som väntar in
100 riktiga millisekunder är både långsamt och *flaky*, eftersom operativsystemet inte
garanterar exakt när koden körs igen.

Med den virtuella hårdvaruplattformen finns det problemet inte. Testet stegar fram tiden själv
genom att skriva till registren, och kan därför verifiera att timeout inträffar exakt vid rätt
tick, varken en för tidigt eller en för sent, på mikrosekunder:

| Register | Roll i drivern |
|---|---|
| `TCCR0A`, `TCCR0B` | Timerläge samt prescaler, sätts vid initiering |
| `TIMSK0` | Aktiverar timeravbrottet |

`driver::timer::Atmega328p` räknar avbrott i mjukvara:
* Varje avbrott motsvarar ett fast intervall (0.128 ms).
* Varje timeout inträffar när tillräckligt många avbrott har räknats. 

Det är den omräkningen mellan millisekunder och antal avbrott som testfilens `getMaxCount()` gör, och det är där ni ska leta om ett timeout-test blir rött.

---
