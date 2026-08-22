# L04 - Enhetstester (del III)

## Dagordning
* Enhetstestning av timer-drivern i det delade [ATmega328p-övningsbiblioteket](../../libs/atmega/README.md).
* Testning av tidsberoende logik utan att faktiskt behöva vänta i tester.
* Resten av lektionen: fortsatt arbete med testsviten i övningsbiblioteket.

---

## Mål med lektionen
* Kunna skriva enhetstester för en driver med tidsberoende beteende, inklusive
  callback-funktionalitet.
* Kunna resonera kring hur man testar tid utan att göra testsviten långsam eller flaky.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_timer_testing.md) om enhetstestning av timer-drivern.

### Under lektionen
* Delta aktivt på genomgången.
* Genomför övningsuppgifterna i [bilaga B](./appendix/b_exercises.md):
  * Ni får tid på er att genomföra uppgiften enskilt.
  * Uppgiften gås sedan igenom i helklass.
* Diskutera i grupp: hur skiljer sig detta från att testa mot en riktig klocka i hårdvaran?

---

## Utvärdering
* Vilka buggar hittade ni i `driver::timer::Atmega328p`, om några?
* Vad är en "flaky test", och varför är sådana tester ett problem i en CI-pipeline?

---

## Nästa lektion
* CI-uppstart, och det praktiska arbetet med **P04** drar i gång i er egen kodbas.

---
