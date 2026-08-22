# Bilaga A - Enhetstester för en GPIO-driver

## Övningsbiblioteket
Från och med den här lektionen övar ni på riktiga drivers i det delade
[ATmega328p-övningsbiblioteket](../../../libs/atmega/README.md), i stället för på egna, små
exempel. Biblioteket innehåller riktiga, färdigskrivna drivers, med medvetet inplanterade
buggar som era tester ska hitta.

`driver::gpio::Interface` täcker läge (`mode()`), läsning, skrivning och toggling av
en logisk nivå (`read()`, `write()`, `toggle()`), initieringsstatus (`isInitialized()`) samt
aktivering av pin change-avbrott (`enableInterrupt()`, `enableInterruptOnPort()`). Den riktiga
implementationen, `driver::gpio::Atmega328p`, skriver och läser hårdvaruregister direkt (t.ex.
`DDRB`, `PORTB`). Genom den virtuella hårdvaruplattformen i biblioteket
([hw_platform.h](../../../libs/atmega/include/arch/test/hw_platform.h)) går detta att
enhetstesta redan nu, helt utan fysisk hårdvara.

## Vad den virtuella hårdvaruplattformen gör
Ett registernamn som `DDRB` är inget annat än ett makro. På riktig hårdvara expanderar det till
en adress i mikrokontrollerns I/O-utrymme; i testbygget (`-DTESTSUITE`) expanderar det i stället
till en plats i en vanlig bytearray:

| Register | I testbygget | Betydelse |
|---|---|---|
| `DDRB` | `test::Memory::data.reg8[2]` | Datariktning: 1 = utgång, 0 = ingång |
| `PORTB` | `test::Memory::data.reg8[5]` | Utnivå, eller pull-up när pinnen är ingång |
| `PINB` | `test::Memory::data.reg8[8]` | Avläst nivå på pinnen |

Drivern är *identisk* i båda fallen, den vet inte om att den skriver till en array. Testet kan
därför både ställa in "hårdvaran" innan anropet och läsa av vad drivern faktiskt skrev efteråt:

```cpp
driver::gpio::Atmega328p led{8U, driver::gpio::Mode::Output};
led.write(true);
EXPECT_TRUE(utils::read(PORTB, 0U));   // pin 8 = PB0, alltså bit 0 i PORTB
```

Notera att `PINB` inte uppdateras av sig själv i testbygget, till skillnad från riktig hårdvara.
Testfilen har därför hjälpfunktioner som `simulateToggle()`, som efterliknar det AVR gör i
hårdvara (en skrivning till `PINx` togglar motsvarande bit i `PORTx`). Behöver ni verifiera
`toggle()` är det den ni ska anropa; annars letar ni efter en driverbugg som inte finns.

---
