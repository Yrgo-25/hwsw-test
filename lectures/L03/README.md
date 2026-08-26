# L03 - Enhetstester (del II)

## Dagordning
* Enhetstestning av serial-drivern i det delade [ATmega328p-övningsbiblioteket](../../libs/atmega/README.md).
* Testning av seriell driver utan fysisk hårdvara.
* Resten av lektionen: fortsatt arbete med testsviten i övningsbiblioteket.

---

## Mål med lektionen
* Kunna skriva enhetstester för en driver som hanterar seriell in- och utdata.
* Kunna resonera kring vilka delar av en seriell driver som är rimliga att enhetstesta redan
  nu, och vilka som kräver mockningstekniken i **L06** eller riktig hårdvara.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_serial_testing.md) om enhetstestning av den seriella drivern.

### Under lektionen
* Delta aktivt på genomgången.
* Genomför övningsuppgifterna i [bilaga B](./appendix/b_exercises.md):
  * Ni får tid på er att genomföra uppgiften enskilt.
  * Uppgiften gås sedan igenom i helklass.
  * Lektionsanteckningar finns [här](./exercises/README.md).

---

## Utvärdering
* Vilka buggar hittade ni i `driver::serial::Atmega328p`, om några?
* Vilka delar av den seriella drivern testade ni, och vilka lämnade ni till senare 
integrationstester? Varför drog ni gränsen där ni gjorde?

---

## Nästa lektion
* Enhetstester (del III): timer-drivern.

---
