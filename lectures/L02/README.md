# L02 - Enhetstester (del I)

## Dagordning
* Från stubb till riktig driver: introduktion till det delade
  [ATmega328p-övningsbiblioteket](../../libs/atmega/README.md).
* Den virtuella hårdvaruplattformen: hur registerskrivningar kan verifieras utan fysisk hårdvara.
* Enhetstestning av GPIO-drivern i övningsbiblioteket, samt jakt på de inplanterade buggarna.
* Kort om kursens projekt **P04**: att ni ska testa er egen kodbas från **P02**/**P03**, och att
  betyget sätts individuellt utifrån Git-historiken. Projektet gås igenom i sin helhet i **L05**,
  när ni öppnar ert eget repo; fram till dess övar ni i övningsbiblioteket.

---

## Mål med lektionen
* Kunna skriva enhetstester för en riktig driver som skriver direkt till hårdvaruregister.
* Kunna verifiera drivern mot den virtuella hårdvaruplattformen, både genom att ställa in
  "hårdvaran" före anropet och läsa av vad drivern skrev efteråt.
* Kunna analysera ett testfel, inklusive en krasch, och avgöra om felet ligger i testet eller i
  drivern.
* Ha en fungerande testsvit för `driver::gpio::Atmega328p`, med hittade buggar åtgärdade.
* Veta vad **P04** går ut på i stora drag, och att era egna commits är underlaget för ert
  individuella betyg.

---

## Instruktioner

### Innan lektionen
* Skumma igenom projektbeskrivningen för [P04](../../projects/P04/README.md) för att se vart
  kursen är på väg. Ni går igenom den i detalj i **L05**.
* Läs [bilaga A](./appendix/a_gpio_testing.md) om enhetstestning av GPIO-drivern.

### Under lektionen
* Delta aktivt på genomgången.
* Genomför övningsuppgifterna i [bilaga B](./appendix/b_exercises.md):
  * Ni får tid på er att genomföra uppgiften enskilt.
  * Uppgiften gås sedan igenom i helklass.
* Lösningsförslag finns [här](./exercises/README.md).

---

## Utvärdering
* Vilka buggar hittade ni i `driver::gpio::Atmega328p`, om några?
* Hur avgjorde ni om ett rött testfall berodde på ert test eller på drivern?
* Varför räcker det att testa `driver::gpio::Atmega328p` mot en virtuell hårdvaruplattform, men
  er egen `driver::gpio::Esp32s3` kräver en annan teknik (se **L06**)?
* Vad innebär det att ett test är deterministiskt, och varför är det viktigt?

---

## Nästa lektion
* Enhetstester (del II): den seriella drivern.

---
