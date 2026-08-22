# **P04** - Test av drivers och ML-algoritmer

## Syfte
Ni ska i grupper skriva en fullständig testsvit för er befintliga kodbas från **P02**
(Driverbibliotek för ESP32-S3) och **P03** (Maskininlärning). Projektet genomförs i samma repo
och med samma grupper som dessa projekt.

**P02** krävde en testbar arkitektur, med tydlig separering mellan interfaces, riktiga
`Esp32s3`-drivers och stubbar, men inga automatiserade tester. **P04** är alltså första gången
ni faktiskt skriver tester mot den kodbasen.

Syftet är att:
* Skriva enhetstester för era riktiga `Esp32s3`-drivers via mockning av ESP-IDF:s C-funktioner,
  minst en driver per gruppmedlem.
* Skriva enhetstester för det ni byggde i **P03**: `ml::lin_reg::Adaptive`,
  `driver::tempsensor::Smart` samt den algoritm ni valde i Fas 2.
* Skriva komponenttester för `system::logic::Logic`, där flera stubbade drivers samverkar.
* Sätta upp en CI-pipeline som bygger firmware samt kör hela testsviten automatiskt vid varje
  push och pull request.
* Öva på att bedöma vad som är rimligt att automatisera respektive verifiera manuellt eller mot
  fysisk hårdvara.

---

## Upplägg och tidsplan
Projektet nämns kort i **L02** och gås igenom i sin helhet i **L05**, då även det praktiska
arbetet i er egen kodbas börjar. Det pågår sedan fram till redovisning i **L19**. Anledningen till
att arbetet börjar först i **L05** är enkel: dessförinnan saknas den teknik som krävs för att testa
er kodbas, eftersom samtliga era `Esp32s3`-drivers går via ESP-IDF (se **L06**). **L02–L04** ägnas
därför helt åt att öva tekniken mot kursens ATmega328p-övningsbibliotek. Arbetet sker sedan löpande
i takt med att nya testnivåer och verktyg introduceras på lektionerna:
* **L02–L04:** Inget arbete i er egen kodbas ännu. Ni övar enhetstestning mot kursens
  ATmega328p-övningsbibliotek, och det är den tekniken ni sedan tillämpar på era egna drivers.
  Skumma igenom den här projektbeskrivningen så att ni vet vart kursen är på väg.
* **L05:** Genomgång av projektet i sin helhet, och fördelning av drivers inom gruppen.
  Testramverket och testbygget på plats i ert eget repo, så att en testsvit kan byggas och köras
  med ett enda kommando, samt uppsättning av CI-pipeline: formatteringskontroll och firmware-bygge
  med artefaktuppladdning.
* **L06:** Analys av **P02**s testbara design, mockning av ESP-IDF:s C-funktioner och därmed era
  första egna enhetstester, samt testjobbet in i pipelinen.
* **L07:** Enhetstester för `driver::tempsensor::Tmp36`s temperaturformel och för
  `ml::lin_reg::Adaptive`, med kända referensvärden och rimliga toleranser, plus tillämpning av
  mockningstekniken från **L06** på fler egna drivers.
* **L08–L09, L11:** Komponenttester för `system::logic::Logic`.
* **L12–L13:** Statisk analys som eget jobb i pipelinen, samt härdning av pipelinen
  (jobbordning, branch protection, caching).
* **L14–L17:** (VG) Mätdatainsamling, processning, visualisering samt HW/SW-integrationstester.
* **L18:** Finputsning och redovisningsförberedelser.
* **L19:** Redovisning och inlämning.

---

## Testnivåer
Testsviten ska vara uppdelad i tydligt separerade nivåer, i linje med testpyramiden som
introduceras i **L01**:

1. **Enhetstester**: testar en enskild klass i isolation. De riktiga `Esp32s3`-drivers testas
   genom att mocka de ESP-IDF-funktioner de anropar (se **L06**), inte mot fysisk hårdvara. Ren
   beräkningslogik, som `driver::tempsensor::Tmp36` och era ML-algoritmer, går att testa direkt,
   med en stubb som testdubbel där ett beroende behövs.
2. **Komponenttester**: testar hur flera klasser samverkar, t.ex. `system::logic::Logic`
   tillsammans med samtliga sex stubbade drivers, eller ett helt neuralt nätverk uppbyggt av
   flera lager. Beroenden som inte testas direkt ersätts med stubbar via respektive interface.
3. **HW/SW-integrationstester (VG)**: verifierar det färdiga systemet mot fysisk `ESP32-S3`-
   hårdvara. Vissa av dessa kan automatiseras (t.ex. via seriell kommunikation och ett
   Python-skript), medan andra rimligen kräver manuell verifiering; dokumentera i så fall vad
   som testats och hur.

Samtliga tester skrivs med testramverket [`yrgo::test`](../../libs/test/README.md).

---

## Krav
Kraven nedan beskriver vad gruppens gemensamma kodbas och testsvit ska innehålla. Det är alltså
ett gemensamt golv som gör att var och en av er har något konkret att bidra till och visa upp;
själva betyget sätts däremot individuellt, se [Bedömning](#bedömning).

### Enhetstester för drivers (G)
Skriv enhetstester för era riktiga `Esp32s3`-drivers genom att mocka de ESP-IDF-funktioner de
anropar (se **L06**). Testerna ska varken kräva fysisk hårdvara eller länka mot ESP-IDF:s riktiga
driverkomponenter för att kunna köras på värddatorn.

**Omfattning: minst en driver per gruppmedlem.** Med 6–8 drivers och fyra medlemmar innebär det
att merparten av driverlagret täcks, och att var och en av er har något eget att visa upp och
förklara vid redovisningen (se [Bedömning](#bedömning)). Fördela driverna mellan er och notera
fördelningen i bidragsrapporten.

Exempel på vad ett drivertest bör verifiera:
* **`driver::gpio::Esp32s3`:** att rätt pin och nivå skickas vidare vid `write()`, och att
  `read()` returnerar det värde mocken matar tillbaka.
* **`driver::serial::Esp32s3`:** att utskriven text hamnar i den mockade UART-bufferten, korrekt
  formaterad.
* **`driver::timer::Esp32s3`:** att timern konfigureras med rätt period, och att ett mockat
  avbrott ger förväntat beteende.
* **Initiering och felhantering:** att drivern hanterar ett misslyckat ESP-IDF-anrop (en returkod
  skild från `ESP_OK`) kontrollerat, i stället för att gå vidare i ett odefinierat tillstånd.

Skriv därutöver enhetstester för `driver::tempsensor::Tmp36`s temperaturformel (se **L07**). Den
går att testa redan innan mockningen är på plats, genom att skicka in en `driver::adc::Stub` som
ger kända insignaler.

### Enhetstester för ML-algoritmer (G)
Skriv enhetstester för det ni byggde i **P03**:
* **`ml::lin_reg::Adaptive`** (Fas 1): att träningen konvergerar, att en tränad modell predikterar
  korrekt för kända, framräknade exempel, samt gränsfall som ogiltig indata och ogiltig
  lärhastighet.
* **`driver::tempsensor::Smart`** (Fas 1): mata in kända råvärden via en `driver::adc::Stub` och
  verifiera den predikterade temperaturen. Notera att `Smart` ärver samma
  `driver::tempsensor::Interface` som `Tmp36`, och därför testas på precis samma sätt.
* **Er valda algoritm från Fas 2**, vilken den än blev: verifiera centrala beräkningssteg,
  träningens beteende samt rimliga gränsfall. Vad som är meningsfullt att testa beror på vilken
  algoritm ni valde; motivera era val av testfall vid redovisningen.

Samtliga dessa tester körs på värddatorn och kräver varken målhårdvara eller mockning av ESP-IDF.

### Komponenttester (G)
Skriv komponenttester för `system::logic::Logic` som verifierar samspelet med dess drivers, med
hjälp av `driver::factory::Stub` och de övriga stubbarna, exempelvis:
* Att serial-kommandot `"on"`/`"off"` resulterar i att `driver::gpio::Stub` sätts till rätt
  nivå.
* Att `"blink on"` tillsammans med en triggad timertimeout togglar LED:en, och att `"blink off"`
  släcker den och inaktiverar togglingen.
* Att `"temp"` respektive `"status"` skriver ut korrekt information via `driver::serial::Stub`,
  baserat på simulerade värden i `driver::tempsensor::Stub`.

### Testautomatisering (G)
Pipelinen sätts upp i **L05**, får sitt testjobb i **L06** och byggs ut löpande under kursen. Vid
inlämning ska den:
* Bygga och köra hela testsviten automatiskt vid varje push och pull request.
* Bygga er firmware för `ESP32-S3` via ESP-IDF, och i samma jobb ladda upp den byggda binären
  som en artefakt (`actions/upload-artifact`), så att den går att hämta från körningen på GitHub. Det kräver ingen fysisk hårdvara eller self-hosted runner, se
  [bilaga A i L05](../../lectures/L05/appendix/a_ci_startup.md) för ett konkret exempel.
  Flashning och körning mot riktig hårdvara ligger utanför CI, se **L17**.
* Kontrollera kodformattering automatiskt, i linje med [ci/format.sh](../../ci/format.sh) i det
  här repot.
* Vara konfigurerad så att en pull request med röda jobb inte går att merga till `main`
  (branch protection, se **L13**).

### Kodkvalitet (G)
* Kör statisk analys (`clang-tidy` och/eller `cppcheck`) på kodbasen och åtgärda relevanta
  varningar.
* All tillagd testkod ska följa befintlig kodstil och vara tydligt strukturerad, en testsvit per
  testad klass.

### Mätdatainsamling och HW/SW-integration (VG)
* Samla in mätdata från systemet på fysisk hårdvara (t.ex. temperaturen över tid via `"temp"`,
  eller tidmätning av blinktogglingens precision) via seriell kommunikation.
* Processa och lagra mätdatan strukturerat (t.ex. som CSV).
* Visualisera mätdatan (t.ex. med Python och `matplotlib`) och jämför resultatet mot systemets
  kravställning.
* Dokumentera minst ett integrationstest som inte rimligen kan automatiseras, samt hur det
  verifierades manuellt.

### Robusthet (VG)
* Identifiera och testa minst tre relevanta edge cases som inte redan täcks av era G-tester,
  t.ex. ogiltiga serial-kommandon (`"period 0"`, ett okänt kommando), eller, om ni implementerat
  EEPROM/watchdog i **P02**, larmgränsen för temperatur respektive att watchdogen faktiskt
  uteblir vid ett felaktigt tillstånd.
* Kör testsviten med AddressSanitizer och/eller UndefinedBehaviorSanitizer och åtgärda eventuella
  fel som hittas.

---

## Användning av AI-verktyg
Ni får använda Claude (eller motsvarande AI-verktyg) till:
* att skriva enhets- och komponenttester samt ESP-IDF-mockar,
* att sätta upp repot och projektet (t.ex. build-konfiguration, mappstruktur, CI-pipeline),
* att förklara felmeddelanden, verktygsutdata och testramverkets API.

Det som bedöms är inte vem som skrev raderna, utan att ni förstår och kan stå för det ni lämnar
in. Två saker följer av det:
* **Granska allt ni committar.** Ett AI-genererat test som ser rimligt ut men inte verifierar
  något, t.ex. för att det saknar assertion eller testar stubben i stället för den riktiga koden,
  är värre än inget test alls: det ger falsk trygghet. Kontrollera att testet faktiskt blir rött
  när koden är trasig, innan ni litar på att det blir grönt när den fungerar.
* **Ni ska kunna förklara varje test ni åberopar** vid redovisningen: vad det verifierar, vilken
  testnivå det tillhör och varför det är utformat som det är (se [Bedömning](#bedömning)).

Ange tydligt i `p04_report.md` var och hur AI-verktyg har använts. Samma regler gäller i **P03**,
med den skillnaden att själva ML-implementationen där ska vara skriven för hand av gruppen.

---

## Versionshantering (Git)
* All kod versionshanteras i samma repo som **P02** och **P03**.
* All utveckling ska ske via branches och pull requests.
* Alla studenter ska bidra med egna commits.
* Läraren ska bjudas in som collaborator och genomför en kod-review på GitHub.

---

## Gruppindelning
* Samma grupper som i **P02** och **P03**.
* Kodbasen och testsviten byggs gemensamt i grupp, men betyget sätts individuellt (se
  [Bedömning](#bedömning)).

### Bidragsrapport (obligatorisk)
Ni lämnar in en gemensam bidragsrapport i filen `p04_report.md` där ni övergripande beskriver
vem som gjorde vad, vilka testnivåer och verktyg ni använde, var och hur AI-verktyg har använts
(se [Användning av AI-verktyg](#användning-av-ai-verktyg)), samt eventuella avgränsningar
(t.ex. vilka integrationstester som inte kunde automatiseras och varför). Utvärderingsfrågorna
besvaras i samma fil (se [Utvärdering](#utvärdering)).

---

## Bedömning
Betyget sätts **individuellt, inte på gruppnivå**. Gruppens gemensamma testsvit (se
[Krav](#krav)) är underlaget som gör bedömningen möjlig, men det som avgör ditt betyg är vad
*du* personligen har bidragit med och kan redogöra för, bedömt utifrån Git-historik,
bidragsrapporten och din förmåga att förklara tester och designbeslut vid redovisning.

**OBS!** En student som inte kan visa tillräckligt eget bidrag riskerar att inte bli godkänd,
även om gruppens gemensamma testsvit uppfyller kraven.

### G (individuellt)
* Git-historiken visar att du personligen har bidragit med fungerande enhets- och/eller
  komponenttester för minst en del av kodbasen (en driver eller en ML-algoritm). Att AI-verktyg
  använts som stöd är tillåtet (se [Användning av AI-verktyg](#användning-av-ai-verktyg)); det
  som räknas är att testerna är dina, att de fungerar och att du kan stå för dem.
* Du kan förklara dina tester: vad de testar, vilken testnivå de tillhör (enhets-, komponent-
  eller integrationstest) och varför de är utformade som de är. Kan du inte förklara ett test
  räknas det inte som ditt bidrag, oavsett vad Git-historiken visar.
* Du kan redogöra för hur testsviten byggs och körs, både lokalt och i CI-pipelinen.
* Gruppens gemensamma testsvit och CI-pipeline uppfyller G-kraven i [Krav](#krav); annars
  finns inget konkret för dig att visa upp och förklara.

### VG (individuellt)
Utöver G-kraven ska du med stor skicklighet och hög precision kunna visa eget bidrag till minst
ett av kursplanens två VG-moment:
* **Säker och robust programvara:** du har själv identifierat och testat relevanta edge cases
  (ogiltig indata, gränsvärden, samtidiga händelser) och/eller åtgärdat fel som hittats av
  statisk analys eller sanitizers, och kan motivera varför just dessa gör systemet mer robust.
* **Ett testramverk för kontinuerlig testning:** du har ansvarat för och drivit fram en väsentlig
  del av CI-pipelinen, teststrukturen eller ESP-IDF-mockningen, och kan förklara hur den
  möjliggör fortsatt testning genom produktens livscykel, inte bara att den finns, utan varför
  den är utformad som den är.

---

## Redovisning
Projektet redovisas för lärare under lektionstid:
* Gemensam genomgång av testsviten: struktur, testnivåer och hur stubbar används.
* Demo av CI-pipelinen som körs automatiskt vid en push.
* (VG) Demo av mätdatainsamling och visualisering på fysisk hårdvara.
* Var och en av er ska individuellt kunna förklara era egna tester och designbeslut, samt
  motivera vad som testas på vilken nivå, se [Bedömning](#bedömning).

---

## Utvärdering
Besvara följande frågor gemensamt i `p04_report.md`, tillsammans med bidragsrapporten:
1. Vilka delar av kodbasen var enklast respektive svårast att skriva enhetstester för, och
   varför?
2. Hur avgjorde ni var gränsen går mellan ett enhetstest och ett komponenttest i er kodbas?
3. Behövde ni göra några ändringar i den befintliga kodbasen från **P02**/**P03** för att göra
   den mer testbar, eller var den redan tillräckligt testbar tack vare arkitekturen ni byggde i
   **P02**? Motivera.
4. Vad var fördelen med att mocka ESP-IDF:s C-funktioner i stället för att testa mot fysisk
   hårdvara, och vad missar man genom att göra det?
5. (Om VG-krav genomförts) Vilket integrationstest kunde inte automatiseras, och hur verifierade
   ni det manuellt i stället?
6. Vad hittade den statiska analysen respektive sanitizers, om något? Hur åtgärdade ni det?
7. Vad lärde ni er av projektet? Vad var lätt respektive svårt?

---
