# Bilaga B - Övningsuppgifter

Övningarna nedan görs i **ert eget repo** och är en del av projektarbetet i **P04**. De gås inte
igenom i helklass efteråt, eftersom varje grupps kodbas ser olika ut; utgå i stället från demot
på genomgången och fråga under lektionen när ni kör fast.

---

## Statisk analys
Kör `clang-tidy` och/eller `cppcheck` mot en del av er kodbas från **P02**/**P03**:
* Läs igenom resultatet. Håller ni med om varningarna, eller verkar någon vara en falsk positiv?
* Välj ut minst en relevant varning och åtgärda den.
* Kör verktyget igen och kontrollera att varningen har försvunnit.

---

## Sanitizers
Bygg om er testsvit med `-fsanitize=address,undefined` och kör den:
* Hittade `ASan` eller `UBSan` något fel?
* Läs felrapporten och försök identifiera var i koden felet uppstod.
* Åtgärda felet och kör sedan testsviten igen.

---

## Låt pipelinen växa
Er pipeline från **L05** kör i dag testsvit, firmware-bygge och formatteringskontroll. Lägg till
analysen ovan som ett eget jobb i `.github/workflows/ci.yml`, så att den körs vid varje push och
pull request i stället för bara när någon råkar komma ihåg det:
* Lägg till ett jobb som kör `clang-tidy` och/eller `cppcheck`, och/eller ett jobb som bygger och
  kör testsviten med `-fsanitize=address,undefined`.
* Verifiera att jobbet blir rött om ni återinför ett fel som ni nyss åtgärdade.
* Fundera på om jobbet ska stoppa pipelinen eller bara varna. En varning som alltid är röd slutar
  snabbt att betyda något; ett jobb som aldrig kan bli rött likaså.

---

## Diskussion
* Vad kan statisk analys respektive sanitizers hitta som era enhets- och komponenttester inte kan, och vice versa?
* Varför är det värdefullt att köra sanitizers mot just testsviten, snarare än mot den färdiga produkten på riktig hårdvara?

---
