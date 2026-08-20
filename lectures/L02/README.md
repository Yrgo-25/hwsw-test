# L02 - Enhetstester (del I)

## Dagordning
* Från stubb till riktig driver: introduktion till det delade
  [ATmega328p-övningsbiblioteket](../../libs/atmega/README.md).
* Den virtuella hårdvaruplattformen: hur registerskrivningar kan verifieras utan fysisk hårdvara.
* Enhetstestning av GPIO-drivern i övningsbiblioteket, samt jakt på de inplanterade buggarna.
* **P04**-start: projektets syfte, upplägg och krav.

---

## Mål med lektionen
* Kunna skriva enhetstester för en riktig driver som skriver direkt till hårdvaruregister.
* Kunna verifiera drivern mot den virtuella hårdvaruplattformen, både genom att ställa in
  "hårdvaran" före anropet och läsa av vad drivern skrev efteråt.
* Kunna analysera ett testfel, inklusive en krasch, och avgöra om felet ligger i testet eller i
  drivern.
* Ha en fungerande testsvit för `driver::gpio::Atmega328p`, med hittade buggar åtgärdade.

---

## Instruktioner

### Innan lektionen
* Läs igenom projektbeskrivningen för [P04](../../projects/P04/README.md).
* Läs [bilaga A](./appendix/a_gpio_testing.md) om enhetstestning av GPIO-drivern.

### Under lektionen
* Delta aktivt på genomgången.
* Genomför övningsuppgifterna i [bilaga B](./appendix/b_exercises.md):
  * Ni får tid på er att genomföra uppgiften enskilt.
  * Uppgiften gås sedan igenom i helklass.

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
* Fortsatt arbete med **P04**.

---
