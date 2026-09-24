# Bilaga A - Stubbar med styrbar utdata

## Mönstret bakom era komponenttester
Era komponenttester för `system::logic::Logic` fungerar för att stubbarna låter testet *styra*
vad beroendet returnerar:
* en simulerad temperatur i `driver::tempsensor::Stub`,
* en triggad timeout i `driver::timer::Stub`,
* ett inmatat kommando i `driver::serial::Stub`.

Testet sätter indata via stubben, kör den riktiga klassen och verifierar sedan resultatet via
stubbarna, t.ex. LED-tillståndet i `driver::gpio::Stub` eller utskriven text i
`driver::serial::Stub`. Utan en styrbar utdata hade testet inte vetat vilket resultat som är det
rätta, och därmed inte kunnat verifiera något.

---

## Att lägga till en styrbar utdata
Saknar en stubb möjligheten att styra sin utdata går den att komplettera, utan att interfacet
ändras. Mönstret består av tre delar:
* **En setter** som lagrar önskad utdata i stubben, t.ex. en temperatur eller ett lagers utdata.
* **En metod som tar bort overriden**, så att stubben återgår till sitt ursprungliga beteende.
* **Interface-metoderna använder den lagrade utdatan** när den är satt, i stället för att räkna
  fram eller skriva över den.

Setter och återställning ingår inte i interfacet. Testet anropar dem därför direkt på
stubb-instansen, vilket är skälet till att er `driver::factory::Stub` behöver exponera de
instanser den skapar.

---

## Samma mönster på praktiskt prov 2
På praktiskt prov 2 ska ni komplettera en lagerstubb i ett konvolutionellt neuralt nätverk med
precis detta: en metod som sätter lagrets utdata, en som tar bort den, samt en `feedforward()`
som inte skriver över en satt utdata. Därefter komponenttestar ni hela nätverket via en factory
som skapar stubbar, på samma sätt som `driver::factory::Stub` skapar stubbarna åt `Logic`.

---
