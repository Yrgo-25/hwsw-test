# Bilaga B - Övningsuppgifter

## Förberedelse
* Läs igenom drivern [gpio/atmega328p.h](../../../libs/atmega/include/driver/gpio/atmega328p.h)
  och dess [källkod](../../../libs/atmega/source/driver/gpio/atmega328p.cpp).
* Läs igenom testfilen [gpio/atmega328p_test.cpp](../../../libs/atmega/test/driver/gpio/atmega328p_test.cpp)
  för att förstå strukturen.

## Skriv tester
* Ta bort `#ifdef LECTURE2` samt motsvarande `#endif` i testfilen för att aktivera testerna.
* Fyll i testfallen enligt kommentarerna i filen, med start i `Gpio_Atmega328p.Initialization`,
  följt av output- och input-testen.
* Bygg och kör testsviten:

  ```bash
  cd libs/atmega/test
  make
  ```

## Buggjakt
* Analysera eventuella testfel: ligger felet i ert test, eller i drivern?
* Åtgärda eventuella buggar i [atmega328p.cpp](../../../libs/atmega/source/driver/gpio/atmega328p.cpp).
* Kör om testsviten tills alla tester blir gröna.

**En krasch är också ett testresultat.** Om testsviten avslutas med *segmentation fault* i
stället för ett rött testfall har ni hittat en riktig bugg, inte gjort något fel. Skillnaden mot
ett vanligt testfel är bara att programmet dör innan ramverket hinner skriva ut något, så ni
måste själva ringa in var det small:
* Kommentera bort delar av testfallet, eller minska intervallet av pinnar som testas, tills
  kraschen försvinner. Det sista värdet ni tog bort är det som utlöser buggen.
* Fundera på vad drivern gör med just det värdet. Ett vanligt mönster i hårdvarunära kod är att
  ett objekt som *inte* kunde initieras ändå försöker städa upp efter sig, och då avrefererar en
  pekare som aldrig sattes.

---

## Tillämpa på er egen kodbas
Samma teknik, ett interface med en riktig implementation och en hårdvaruoberoende stubb, gäller
för er egen `driver::gpio::Interface` från **P02**. Skillnaden är att er `driver::gpio::Esp32s3`
anropar ESP-IDF:s C-funktioner i stället för att skriva direkt till register, vilket kräver en
annan mockningsteknik. Den går vi igenom i **L06**, och det är då ni börjar skriva enhetstester
för era egna drivers i **P04**.

Er `driver::gpio::Stub` fyller en annan roll: den är inte något ni testar, utan en **testdubbel**
ni testar *med*, när ni testar kod som beror på en GPIO-driver.

Fram till **L06** är det alltså övningsbiblioteket ni arbetar i, inte er egen kodbas. Det är inte
en tillfällighet: samtliga era `Esp32s3`-drivers går via ESP-IDF, och tekniken för att komma runt
det kommer först då. Tiden fram till dess använder ni till att bli varma i kläderna på riktiga
drivers, om än på en annan mikrokontroller.

---

## Diskussion
* Vilka buggar hittade ni i `Atmega328p`-drivern? Hur hade de påverkat ett system i produktion?
* Vad tycker ni om den virtuella hårdvaruplattformen som teknik, jämfört med att testa mot
  riktig hårdvara?
* `driver::gpio::Esp32s3` är inte mycket mer än ett tunt lager ovanpå `gpio_set_level()`/
  `gpio_get_level()`. Vad krävs för att kunna enhetstesta att just det lagret är korrekt? Se
  **L06**.

---
