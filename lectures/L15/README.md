# L15 - Projektarbete (del II)

## Dagordning
* Arbete med **P04**, med fokus på att färdigställa allt som körs på värddatorn: enhetstester,
  komponenttester, CI-pipeline och statisk analys.
* Förberedelse inför **L16–L20**, som handlar om mätdatainsamling och testautomatisering med
  Python mot fysisk hårdvara.

---

## Mål med lektionen
* Ha samtliga G-krav i **P04** som inte kräver fysisk hårdvara uppfyllda:
  [Enhetstester för drivers](../../projects/P04/README.md#enhetstester-för-drivers-g),
  [Enhetstester för ML-algoritmer](../../projects/P04/README.md#enhetstester-för-ml-algoritmer-g),
  [Komponenttester](../../projects/P04/README.md#komponenttester-g),
  [Testautomatisering](../../projects/P04/README.md#testautomatisering-g) samt
  [Kodkvalitet](../../projects/P04/README.md#kodkvalitet-g).
* Ha en firmware som går att flasha till `ESP32-S3` och kommunicera med över seriell
  kommunikation, så att arbetet mot hårdvaran kan börja direkt i **L16**.

---

## Instruktioner
* Stäm av i gruppen mot kraven ovan: vad saknas, och vem tar vad?
* Prioritera det som saknas för G före VG-kraven.
* Kontrollera inför **L16** att varje grupp har:
  * Ett `ESP32-S3`-kort med USB-kabel.
  * En firmware byggd från `main`, gärna den artefakt CI-pipelinen laddar upp (se **L05**).
  * Fungerande seriell kommunikation: flasha kortet, skicka t.ex. `"status"` via `idf.py monitor`
    och kontrollera att svaret kommer tillbaka.
* Använd lektionstiden för att ställa frågor och be om granskning av era pull requests.

---

## Utvärdering
* Uppfyller er testsvit och CI-pipeline nu samtliga G-krav i **P04** som inte kräver hårdvara? Om
  inte, vad återstår och när gör ni det?
* Siktar någon i gruppen på VG, och vilka av VG-kraven planerar ni i så fall att ta er an?

---

## Nästa lektion
* **L16–L20** hålls av en gästlärare.
* Mätdatainsamling (del I): insamling och lagring av mätdata från en DUT.

---
