# L02 - Enhetstester (del I)

## Dagordning
* Struktur på ett enhetstest: Arrange-Act-Assert.
* Genomgång av `TEST()` samt `EXPECT_*`-makrona i `yrgo::test`.
* Enhetstestning av GPIO-drivern i det delade [ATmega328p-övningsbiblioteket](../../libs/atmega/README.md).
* **P04**-start: projektets syfte, upplägg och krav.

---

## Mål med lektionen
* Kunna strukturera ett enhetstest enligt Arrange-Act-Assert.
* Kunna skriva enhetstester för en enskild klass med `yrgo::test`.
* Ha en fungerande testsvit för `driver::gpio::Atmega328p`, med eventuella buggar i drivern
  åtgärdade.

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
* Vad innebär Arrange-Act-Assert, och varför är strukturen användbar när man läser andras tester?
* Vilka buggar hittade ni i `driver::gpio::Atmega328p`, om några?
* Varför räcker det att testa `driver::gpio::Atmega328p` mot en virtuell hårdvaruplattform, men
  er egen `driver::gpio::Esp32s3` kräver en annan teknik (se **L06**)?
* Vad innebär det att ett test är deterministiskt, och varför är det viktigt?

---

## Nästa lektion
* Enhetstester (del II): den seriella drivern.
* Fortsatt arbete med **P04**.

---
