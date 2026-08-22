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

## Att automatisera hårdvarutestning: self-hosted runner (demo)
Pipelinen från **L05**/**L13** kör på en GitHub-hostad runner (`runs-on: ubuntu-latest`), en
virtuell maskin utan något kort inkopplat. Den kan bygga firmwaren och spara den som artefakt, men
inte flasha den. Demot i **L05** körde på en self-hosted runner, men skillnaden var då bara var
maskinen stod, inte vad den kunde göra.

Gränsen går dock inte vid tekniken, utan vid var runnern står. GitHub Actions kan lika gärna köra
jobb på en maskin ni själva äger, en **self-hosted runner**. Står den maskinen med en `ESP32-S3`
inkopplad via USB kan ett jobb hämta firmware-artefakten, flasha kortet med `esptool.py` och köra
ett testskript mot den seriella porten, helt automatiskt vid varje push. Det kallas
*hardware-in-the-loop* (HIL) och är så här hårdvarutestning automatiseras i industrin.

Att registrera runnern tar bokstavligen någon minut, och ni såg det göras i **L05**: maskinen
registreras under *Settings → Actions → Runners* och får en egen etikett, som jobbet sedan begär
via `runs-on:`. Det som gör tekniken krävande är allt därefter:
* **Maskinen måste vara igång.** Är den avstängd när någon pushar blir jobbet stående i kö, precis
  som ni såg demonstrerat i **L05**. En pipeline som fastnar eller blir röd av skäl som inte har med
  koden att göra slutar snabbt att tas på allvar.
* **Hårdvaran måste vara i ett känt utgångsläge.** Ett kort som hängt sig sedan förra körningen
  gör nästa testresultat värdelöst.
* **Säkerhet.** En self-hosted runner kör kod från repot på er egen maskin. På ett publikt repo
  kan vem som helst öppna en pull request och därmed köra godtycklig kod hos er, vilket är
  anledningen till att GitHub avråder från self-hosted runners på publika repon.

Vi demonstrerar upplägget under lektionen. Det ingår inte i kraven för **P04**, dels för att det
förutsätter att varje grupp har ett kort permanent inkopplat till en maskin som alltid är igång,
dels för att den manuella dokumentationen (se ovan) är det ni faktiskt bedöms på. Poängen är att
ni ska veta att gränsen mellan "automatiserbart" och "kräver en människa" går att flytta, och vad
det kostar att flytta den.

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
