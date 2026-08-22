# Bilaga B - Övningsuppgifter

## Förberedelse
* Läs igenom drivern [timer/atmega328p.h](../../../libs/atmega/include/driver/timer/atmega328p.h)
  och dess [källkod](../../../libs/atmega/source/driver/timer/atmega328p.cpp).
* Läs igenom testfilen [timer/atmega328p_test.cpp](../../../libs/atmega/test/driver/timer/atmega328p_test.cpp)
  för att förstå strukturen.

## Skriv tester
* Ta bort `#ifdef LECTURE4` samt motsvarande `#endif` i testfilen för att aktivera testerna.
* Fyll i testfallen enligt kommentarerna i filen. **Hjälpfunktionerna** för callback-testningen
  (`callbackInvoked`, `resetCallbackFlag()`, `testCallback()`, `getMaxCount()`) ligger i ett eget
  `#ifdef CALLBACK`-block. De används av *två* testfall, `Callback` och `Restart`; ta bort
  blocket när ni börjar skriva dem.
* Bygg och kör testsviten:

  ```bash
  cd libs/atmega/test
  make
  ```

## Buggjakt
* Analysera eventuella testfel: ligger felet i ert test, eller i drivern?
* Åtgärda eventuella buggar i [atmega328p.cpp](../../../libs/atmega/source/driver/timer/atmega328p.cpp).
* Kör om testsviten tills alla tester blir gröna.

**En krasch är också ett testresultat**, precis som i **L02** (se
[bilaga B i L02](../../L02/appendix/b_exercises.md#buggjakt)). Testfallet som skapar en fjärde
timer, utöver de tre kretsar hårdvaran har, är den mest sannolika platsen: vad händer med en
timer som aldrig blev initierad när den går ur scope?

---

## Tillämpa på er egen kodbas
Er egen `driver::timer::Esp32s3` från **P02** anropar ESP-IDF:s C-funktioner i stället för att
skriva direkt till register, vilket kräver mockningstekniken i **L06**. Det är där, och inte här,
ni skriver enhetstester för era egna drivers i **P04**. Er `driver::timer::Stub` används under
tiden som **testdubbel** när ni testar kod som beror på en timer (se
[bilaga B i L01](../../L01/appendix/b_exercises.md) för samma *princip* i litet format; notera
att övningens `driver::timer::Interface` är en förenklad variant med ett annat API än det
riktiga).

---

## Diskussion
* Stötte ni på några buggar i hur timeout eller callback hanterades? Hur hade ett sådant fel
  märkts av i ett system som förlitar sig på exakt timing?
* Vad är ett *flaky test*, och varför är det viktigt att en timer-drivers tester inte förlitar
  sig på riktig, förfluten tid?

---
