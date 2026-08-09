# L13 - Testautomatisering och kontinuerlig integration (CI)

## Dagordning
* Varför automatisera testkörning i stället för att köra tester manuellt?
* Kontinuerlig integration (CI): grundprinciper.
* GitHub Actions: sätta upp en pipeline som bygger och kör testsviten automatiskt.
* Fortsatt arbete med **P04**.

---

## Mål med lektionen
* Kunna förklara vad kontinuerlig integration innebär och varför det är värdefullt under en
  produkts livscykel.
* Kunna sätta upp en GitHub Actions-pipeline som bygger och kör en C++-testsvit automatiskt vid
  push och pull request.
* Kunna lägga till en formatteringskontroll (`clang-format`, se **L05**) som ett eget jobb i
  pipelinen.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_ci.md) om kontinuerlig integration med GitHub Actions.

### Under lektionen
* Delta aktivt på genomgången.
* Titta på [.github/workflows/ci.yml](../../.github/workflows/ci.yml) i det här repot som
  referens.
* Genomför övningsuppgifterna i [bilaga B](./appendix/b_exercises.md):
  * Ni får tid på er att genomföra uppgiften enskilt eller i grupp.
  * Uppgiften gås sedan igenom i helklass.

---

## Utvärdering
* Vad händer i er pipeline om någon pushar kod som får ett test att misslyckas?
* Varför är det värdefullt att köra tester automatiskt vid varje pull request, i stället för att
  bara köra dem lokalt innan man mergar?

---

## Nästa lektion
* Mätdatainsamling (del I).
* Fortsatt arbete med **P04**.

---
