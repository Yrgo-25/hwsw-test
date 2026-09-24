# L11 - Komponenttester (del III): systemlogiken

## Dagordning
* Genomgång av provet från **L10**.
* Stubbar med styrbar utdata: det mönster komponenttesterna för systemlogiken bygger på.
* Komponenttestet `logic_test.cpp` i ATmega328p-övningsbiblioteket, med stubbarna från
  **L08–L09**.
* Komponenttester för er egen `system::logic::Logic` i **P04**.

---

## Mål med lektionen
* Kunna förklara hur en stubb med styrbar utdata gör det möjligt att komponenttesta en klass med
  kända, kontrollerade indata.
* Kunna sätta ihop `Logic` med en `driver::factory::Stub` och komma åt de skapade
  stubb-instanserna i ett test.
* Kunna verifiera samspelet mellan `Logic` och dess drivers, t.ex. att kommandot `"on"`
  resulterar i att `driver::gpio::Stub` sätts till rätt nivå.
* Ha påbörjat komponenttesterna för er egen `Logic`, med minst ett scenario per gruppmedlem.

---

## Instruktioner

### Innan lektionen
* Repetera [bilaga A i L08](../L08/appendix/a_component_tests.md) om komponenttester.
* Läs [bilaga A](./appendix/a_controllable_stubs.md) om stubbar med styrbar utdata.

### Under lektionen
* Delta aktivt på genomgången av provet.
* Genomför övningsuppgifterna i [bilaga B](./appendix/b_exercises.md):
  * Uppvärmningen i övningsbiblioteket gås igenom i helklass.
  * Komponenttesterna för er egen `Logic` är projektarbete i **P04**, och gås inte igenom i
    helklass.

---

## Utvärdering
* Vad testar ett komponenttest som ett enhetstest inte fångar, och tvärtom?
* Hur exponerar er `driver::factory::Stub` de stubb-instanser den skapar, så att testet kan
  styra och läsa av dem?
* Vilka av era stubbar har en styrbar utdata, och hur skulle ni lägga till en i en stubb som
  saknar det?

---

## Nästa lektion
* Komponenttester (del IV): gränsfall, robusthet och sanitizers.
* Fortsatt arbete med **P04**.

---
