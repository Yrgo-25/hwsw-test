# L13 - Testautomatisering och kontinuerlig integration (CI)

## Dagordning
* Varför automatisera testkörning i stället för att köra tester manuellt?
* Kontinuerlig integration (CI): grundprinciper.
* Anatomin i den pipeline ni satte upp i **L05**: triggers, jobb och steg.
* Att härda pipelinen: caching, `concurrency`, samt branch protection så att ett rött jobb
  faktiskt hindrar en merge.
* Vad som inte hör hemma i CI, och varför.
* Fortsatt arbete med **P04**.

---

## Mål med lektionen
* Kunna förklara vad kontinuerlig integration innebär och varför det är värdefullt under en
  produkts livscykel.
* Kunna läsa gruppens egen `ci.yml` och förklara vad varje trigger, jobb och steg gör.
* Kunna härda en befintlig pipeline: cacha det som är dyrt att bygga om, och konfigurera
  branch protection så att en pull request inte går att merga med röda jobb.
* Kunna motivera vad som hör hemma i pipelinen respektive vad som kräver manuell verifiering
  eller fysisk hårdvara.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_ci.md) om kontinuerlig integration med GitHub Actions.
* Ta fram gruppens egen `.github/workflows/ci.yml` från **L05**, ni utgår från den under
  lektionen.

### Under lektionen
* Delta aktivt på genomgången, där härdningen demonstreras live på ett minimalt projekt.
* Titta på [.github/workflows/ci.yml](../../.github/workflows/ci.yml) i det här repot som
  referens.
* Genomför därefter övningsuppgifterna i [bilaga B](./appendix/b_exercises.md), i grupp och i ert
  eget repo.
* **Lösningen gås inte igenom i helklass efteråt.** Att tillämpa det här på gruppens egen kodbas
  är projektarbete i **P04**: varje grupps repo ser olika ut, och det är i genomförandet ni lär
  er tekniken. Demot från genomgången är mallen ni utgår från, och jag går runt och hjälper till
  under tiden.

---

## Utvärdering
* Vad händer i er pipeline om någon pushar kod som får ett test att misslyckas?
* Varför är det värdefullt att köra tester automatiskt vid varje pull request, i stället för att
  bara köra dem lokalt innan man mergar?
* Vad hindrar i dag någon i gruppen från att merga en pull request med rött kryss, och vad
  behöver konfigureras för att faktiskt hindra det?
* Vilka delar av er testning kan pipelinen aldrig utföra, och varför?

---

## Nästa lektion
* Mätdatainsamling (del I).
* Fortsatt arbete med **P04**.

---
