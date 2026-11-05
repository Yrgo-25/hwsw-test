# L18 - Automatiserade HW/SW-integrationstester med Python

## Dagordning
* Genomgång: HW/SW-integrationstester som en egen testnivå, och hur de skiljer sig från
  komponenttesterna i **L11**.
* `pytest` som testramverk för integrationstester mot fysisk hårdvara: struktur, fixtures och ett
  känt utgångsläge.
* Instabila tester, samt hur integrationstester kan köras automatiskt via en self-hosted runner.
* Arbete med integrationstestdelen av **P04**.

---

## Mål med lektionen
* Kunna förklara vad ett HW/SW-integrationstest kan hitta som ett komponenttest inte kan, och
  tvärtom.
* Kunna skriva automatiserade integrationstester i Python med `pytest`, som skickar kommandon till
  `ESP32-S3` över seriell kommunikation och verifierar svaret.
* Kunna säkerställa ett känt utgångsläge inför varje testfall, och känna igen de vanligaste
  orsakerna till instabila tester.
* Ha minst ett automatiserat integrationstest på plats i ert eget repo.

---

## Instruktioner

### Innan lektionen
* Läs [bilaga A](./appendix/a_automated_integration.md) om automatiserade
  HW/SW-integrationstester med Python.
* Ta med ert `ESP32-S3`-kort och en USB-kabel.

### Under lektionen
* Delta aktivt på genomgången.
* Genomför därefter uppgifterna i [bilaga B](./appendix/b_exercises.md), i grupp och i ert eget
  repo. De gås inte igenom i helklass; läraren går runt och hjälper till under tiden.

---

## Utvärdering
* Vilket integrationstest skrev ni, och vilket komponenttest från **L11** motsvarar det?
* Hur säkerställer er svit att varje testfall startar från ett känt utgångsläge?
* Vad hade krävts för att köra er svit automatiskt vid varje push?

---

## Nästa lektion
* Manuella integrationstester och projektarbete (del III).

---
