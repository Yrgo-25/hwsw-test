# Bilaga B - Uppgifter

Uppgifterna nedan görs i **ert eget repo** och är en del av projektarbetet i **P04**. De gås inte
igenom i helklass efteråt, eftersom varje grupps kodbas ser olika ut; utgå i stället från demot
på genomgången och fråga under lektionen när ni kör fast.

---

## Statisk analys
Kör `clang-tidy` och/eller `cppcheck` mot er kodbas från **P02**/**P03**:
* Läs igenom resultatet. Håller ni med om varningarna, eller verkar någon vara en falsk positiv?
* Åtgärda de relevanta varningarna, och kör verktyget igen för att kontrollera att de har
  försvunnit.
* Frivilligt: lägg till analysen som ett eget jobb i `.github/workflows/ci.yml`.

---

## Branch protection
Konfigurera `main` så att en pull request inte går att merga förrän pipelinens jobb har lyckats,
enligt bilaga A. Testa genom att öppna en pull request med ett medvetet trasigt test och
kontrollera att merge-knappen faktiskt blockeras.

---

## Ordningen mellan jobben
Bestäm var testjobbet (och ett eventuellt analysjobb) hör hemma i kedjan, och motivera valet
utifrån hur lång tid jobben tar. Tiderna syns per jobb under *Actions*.

---
