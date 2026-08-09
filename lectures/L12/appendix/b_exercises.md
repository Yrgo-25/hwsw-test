# Bilaga B - Övningsuppgifter

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

## Diskussion
* Vad kan statisk analys respektive sanitizers hitta som era enhets- och komponenttester inte kan, och vice versa?
* Varför är det värdefullt att köra sanitizers mot just testsviten, snarare än mot den färdiga produkten på riktig hårdvara?

---
