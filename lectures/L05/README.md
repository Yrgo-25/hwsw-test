# L05 - Testbar design

## Dagordning
* Varför är vissa kodbaser lättare att testa än andra?
* Interfaces som gräns mellan logik och hårdvara/beroenden.
* Dependency injection: direkt via konstruktorn, och via en factory (som i **P02**).
* Kodformattering: `clang-format` och en delad formatteringskonfiguration.
* Resten av lektionen: fortsatt arbete med **P04**.

---

## Mål med lektionen
* Kunna förklara varför testbarhet är en designegenskap, inte något som tillkommer i efterhand.
* Kunna förklara hur `driver::factory::Interface` gör det möjligt att växla mellan
  `system::logic::Logic` i hårdvaruläge och simulerat läge utan att ändra `Logic` själv.
* Kunna resonera kring vad som hade behövt ändras om en klass i stället skapade sina beroenden
  internt.
* Kunna köra `clang-format` mot sin kodbas och förklara vad en delad formatteringskonfiguration
  löser.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_testable_design.md) om testbar design.

### Under lektionen
* Delta aktivt på genomgången.
* Gå igenom hur `system::logic::Logic` respektive `driver::tempsensor::Tmp36` tar emot sina
  beroenden i **P02**, och varför det gör dem testbara.
* Genomför övningsuppgifterna i [bilaga B](./appendix/b_exercises.md):
  * Ni får tid på er att genomföra uppgiften enskilt.
  * Uppgiften gås sedan igenom i helklass.
* Lektionens innehåll är förhållandevis kort. Resterande tid ägnas åt fortsatt arbete med
  **P04**, t.ex. enhetstester för era egna stubbar (**L02–L04**) eller analys av var i er
  kodbas testbar design redan tillämpas.

---

## Utvärdering
* Vad är skillnaden mellan en klass som tar emot sina beroenden via konstruktorn (via referenser
  eller via en factory), och en klass som skapar dem internt, sett till testbarhet?
* Varför tar er `Logic` emot en hel factory i stället för sina drivers en och en?
* Varför är det värdefullt att hela gruppen använder samma `clang-format`-konfiguration, i
  stället för att var och en formatterar sin kod efter eget tycke?

---

## Nästa lektion
* Stubbar och virtuell hårdvara.
* Fortsatt arbete med **P04**.

---
