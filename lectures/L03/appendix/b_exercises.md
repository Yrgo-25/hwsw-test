# Bilaga B - Övningsuppgifter

## Förberedelse
* Läs igenom drivern [serial/atmega328p.h](../../../libs/atmega/include/driver/serial/atmega328p.h)
  och dess [källkod](../../../libs/atmega/source/driver/serial/atmega328p.cpp).
* Läs igenom testfilen [serial/atmega328p_test.cpp](../../../libs/atmega/test/driver/serial/atmega328p_test.cpp)
  för att förstå strukturen.

## Skriv tester
* Ta bort `#ifdef LECTURE3` samt motsvarande `#endif` i testfilen för att aktivera testerna.
* Fyll i testfallen enligt kommentarerna i filen.
* Bygg och kör testsviten:

  ```bash
  cd libs/atmega/test
  make
  ```

## Buggjakt
* Analysera eventuella testfel: ligger felet i ert test, eller i drivern?
* Åtgärda eventuella buggar i [atmega328p.cpp](../../../libs/atmega/source/driver/serial/atmega328p.cpp).
* Kör om testsviten tills alla tester blir gröna.

---

## Tillämpa på er egen kodbas
Er egen `driver::serial::Esp32s3` från **P02** anropar ESP-IDF:s C-funktioner i stället för att
skriva direkt till register, vilket kräver mockningstekniken i **L06**. Er `driver::serial::Stub`
är däremot redan enhetstestbar nu, precis som `Atmega328p`-drivern ovan. Att skriva dessa tester
för er egen kodbas är en del av **P04**.

---

## Diskussion
* Stötte ni på några buggar relaterade till hur `UDR0`/`UCSR0A` hanterades? Hur hade ett sådant
  fel yttrat sig i en verklig seriell kommunikation, t.ex. mot en dator eller en annan enhet?
* Den faktiska seriella överföringen (baud rate, elektriska nivåer) går inte att verifiera med
  ett enhetstest, varken här eller för er egen kodbas. Vilken testnivå senare i kursen är bättre
  lämpad för det?

---
