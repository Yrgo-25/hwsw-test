# Bilaga B - Övningsuppgifter

Uppgiften nedan görs i grupp och i **ert eget repo**, på riktig hårdvara. Den gås inte igenom i
helklass; läraren går runt och hjälper till under tiden.

---

## Manuellt testfall
Välj ut ett integrationsscenario för ert eget system som inte går att verifiera via den seriella
porten, till exempel att det seriella kommandot `"temp"` på riktig hårdvara returnerar en rimlig
temperatur jämfört med en referenstermometer. Finns ingen referenstermometer tillgänglig, välj ett
scenario som bara kräver `ESP32-S3`, t.ex. att LED:en synligt tänds och släcks med rätt period.

Skriv ett fullständigt manuellt testfall enligt mallen i [bilaga A](./a_manual_integration.md),
t.ex. i `docs/manual_tests.md`. Testfallet ska minst ange:
* **Syfte:** Vad ska testet verifiera, och vilket krav hör det till?
* **Förutsättningar:** Vilken hårdvara, mjukvara och eventuell mätutrustning krävs?
* **Genomförande:** Exakt vilka steg ska utföras, och i vilken ordning?
* **Förväntat resultat:** Vad måste inträffa för att testet ska godkännas?
* **Faktiskt resultat:** Vad hände när ni genomförde testet?
* **Bedömning:** Blev testet godkänt eller underkänt?

Genomför testet och dokumentera resultatet. Det dokumenterade testfallet ingår i VG-momentet
*Ett testramverk för kontinuerlig testning* i **P04**.

---

## Spårbarhetstabell
* Gör en spårbarhetstabell för ert system enligt [bilaga A](./a_manual_integration.md), med
  kraven i **P04** och era egna mätkrav från **L17** som rader.
* Finns det krav som saknar test? Finns det krav som bara verifieras manuellt men som går att
  automatisera?

---

## Diskussion
* Varför är det värt att dokumentera ett manuellt test lika noggrant som ett automatiserat,
  trots att det kräver en människa varje gång?
* Vad hade krävts för att automatisera just ert testfall, och hade det varit värt besväret?

---
