# Bilaga A - HW/SW-integrationstestning och robust mjukvara

## Vad kan automatiseras, vad kräver en människa?
Enhets- och komponenttester körs på värddatorn, utan hårdvara, och kan därför köras automatiskt
i CI (**L13**) vid varje push. **HW/SW-integrationstester** verifierar i stället det färdiga
systemet på riktig `ESP32-S3`-hårdvara, och här är gränsen för vad som går att automatisera
mindre självklar:
* Ett test som skickar kommandon över seriell kommunikation och verifierar svaret (se **L14**)
  går ofta att automatisera med ett Python-skript.
* Ett test som kräver att en människa fysiskt trycker på en knapp, mäter en spänning med ett
  instrument, eller bedömer om en lysdiod faktiskt lyser med rätt styrka, kräver manuell
  verifiering.

Det är fullt rimligt, och förväntat, att en del av er integrationstestning i **P04** är manuell.
Det viktiga är att den då **dokumenteras** på ett spårbart sätt.

---

## Mall för ett manuellt testfall
Ett manuellt testfall bör dokumenteras minst lika tydligt som ett automatiserat, annars kan
det inte upprepas på samma sätt av någon annan (eller av er själva om ett halvår):
* **Syfte.** Vad ska testfallet verifiera?
* **Förutsättningar.** Vilken hårdvara, firmware-version och utrustning krävs, och i vilket
  utgångsläge ska systemet vara?
* **Genomförande.** Exakt vilka steg utfördes, i vilken ordning?
* **Förväntat resultat.** Vad *skulle* hända, formulerat innan testet körs?
* **Faktiskt resultat.** Vad hände i praktiken, med uppmätta värden?
* **Bedömning.** Godkänt eller underkänt, och vem som utförde testet när.

Det är de sex fälten ni ska använda i övningsuppgiften i [bilaga B](./b_exercises.md).

---

## Säker och robust mjukvara
Ett system som styr fysisk hårdvara bör hantera ogiltig indata och oväntade tillstånd på ett
kontrollerat sätt, i stället för att fortsätta i ett odefinierat tillstånd. Några principer:
* **Validera indata vid systemets gränser**, t.ex. värden som kommer från en sensor eller från
  seriell kommunikation, snarare än att lita blint på att de alltid är rimliga.
* **Markera funktioner `noexcept`** där de rimligen inte ska kasta undantag, och undvik
  undantag (`throw`/`try`/`catch`) i hårdvarunära kod överlag, i linje med kursens kodbaser.
* **Faila högt, inte tyst.** Ett tydligt felmeddelande och ett kontrollerat avslut
  (`std::terminate()`) vid ett allvarligt, orimligt tillstånd är ofta bättre än att låta
  programmet fortsätta i ett odefinierat läge.
* **Kontrollera returvärden.** Ignorera inte felkoder från hårdvara eller operativsystem. Ett misslyckat anrop bör hanteras direkt, inte upptäckas långt senare.

---
