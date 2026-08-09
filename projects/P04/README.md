# **P04** - Test av drivers och ML-algoritmer

## Syfte
Ni ska i grupper skriva en fullständig testsvit för er befintliga kodbas från **P02**
(Driverbibliotek för ESP32-S3) och **P03** (Maskininlärning). Projektet genomförs i samma repo
och med samma grupper som dessa projekt.

**P02** krävde en testbar arkitektur, med tydlig separering mellan interfaces, riktiga
`Esp32s3`-drivers och stubbar, men inga automatiserade tester. **P04** är alltså första gången
ni faktiskt skriver tester mot den kodbasen.

Syftet är att:
* Skriva enhetstester för era sex `Stub`-klasser från **P02** (adc, factory, gpio, serial,
  tempsensor, timer), samt för minst en av de riktiga `Esp32s3`-drivers via mockning av
  ESP-IDF:s C-funktioner.
* Skriva enhetstester för era ML-algoritmer (**P03**): den adaptiva regressionsmodellen samt
  minst ett neuralt nätverkslager.
* Skriva komponenttester för `system::logic::Logic`, där flera stubbade drivers samverkar.
* Sätta upp en CI-pipeline som bygger firmware samt kör hela testsviten automatiskt vid varje
  push och pull request.
* Öva på att bedöma vad som är rimligt att automatisera respektive verifiera manuellt eller mot
  fysisk hårdvara.

---

## Upplägg och tidsplan
Projektet påbörjas i **L02**, direkt efter att grunderna i enhetstestning med `yrgo::test`
introducerats, och pågår genom hela kursen fram till redovisning i **L19**. Arbetet sker
löpande i takt med att nya testnivåer och verktyg introduceras på lektionerna:
* **L02–L04:** Enhetstester för era befintliga GPIO-, seriell- samt timer-stubbar. Övriga tre
  stubbar (adc, tempsensor, factory) följer samma mönster och skrivs löpande.
* **L05–L06:** Analys av **P02**s testbara design samt mockning av ESP-IDF:s C-funktioner.
* **L07:** Enhetstester för `Tmp36`s temperaturformel samt för era ML-algoritmer från **P03**,
  plus tillämpning av mockningstekniken från **L06** på fler egna drivers.
* **L08–L09, L11:** Komponenttester för `system::logic::Logic`.
* **L12–L13:** Statisk analys samt CI-pipeline (inklusive firmware-bygge).
* **L14–L17:** (VG) Mätdatainsamling, processning, visualisering samt HW/SW-integrationstester.
* **L18:** Finputsning och redovisningsförberedelser.
* **L19:** Redovisning och inlämning.

---

## Testnivåer
Testsviten ska vara uppdelad i tydligt separerade nivåer, i linje med testpyramiden som
introduceras i **L01**:

1. **Enhetstester**: testar en enskild klass i isolation. Stubbarna testas direkt (de är redan
   hårdvaruoberoende); de riktiga `Esp32s3`-drivers testas genom att mocka de
   ESP-IDF-funktioner de anropar (se **L06**), inte mot fysisk hårdvara.
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
Skriv enhetstester för era sex `Stub`-klasser från **P02**, exempelvis:
* **`driver::gpio::Stub`:** korrekt starttillstånd samt att `write()`/`read()` fungerar.
* **`driver::serial::Stub`:** att inmatade rader returneras korrekt, och att utskriven text går
  att läsa av i testet.
* **`driver::timer::Stub`:** att timeout indikeras vid rätt tidpunkt, varken för tidigt eller
  för sent.
* **`driver::adc::Stub` samt `driver::tempsensor::Stub`:** att simulerade värden returneras
  korrekt.
* **`driver::factory::Stub`:** att den skapar rätt typ av drivers, och att era komponenttester
  (se nedan) kan komma åt de skapade instanserna.

Skriv därutöver enhetstester för `driver::tempsensor::Tmp36`s temperaturformel (se **L07**), samt
för minst en av de riktiga `Esp32s3`-drivers (t.ex. `driver::gpio::Esp32s3`) genom att mocka
de ESP-IDF-funktioner den anropar (se **L06**). De sistnämnda testerna ska varken kräva fysisk
hårdvara eller länka mot ESP-IDF:s riktiga driverkomponenter för att köras på värddatorn.

### Enhetstester för ML-algoritmer (G)
Skriv enhetstester för `ml::lin_reg::Adaptive` samt minst ett av era neurala nätverkslager från
**P03** (t.ex. `Dense`, eller ett CNN-lager om ni implementerade det i Fas 2). Om er grupp
implementerade `driver::tempsensor::Smart` i **P03**, skriv enhetstester för den också, med en
`driver::adc::Stub` för att ge kända, simulerade insignaler. Verifiera bland annat gränsfall
(ogiltig indata, ogiltig lärhastighet) samt att beräkningar ger korrekt resultat för kända,
framräknade exempel.

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
* Sätt upp en CI-pipeline (GitHub Actions) som bygger och kör hela testsviten automatiskt vid
  varje push och pull request.
* Samma pipeline ska även bygga er firmware för `ESP32-S3` via ESP-IDF, som ett separat jobb.
  Det kräver ingen fysisk hårdvara eller självhostad runner, se [bilaga A i L13](../../lectures/L13/appendix/a_ci.md#att-bygga-esp-idf-firmware-i-ci)
  för ett konkret exempel. Flashning och körning mot riktig hårdvara ligger utanför CI, se
  **L17**.
* Kodformattering ska kontrolleras automatiskt i samma pipeline, i linje med
  [ci/format.sh](../../ci/format.sh) i det här repot.

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
Ni lämnar in en gemensam bidragsrapport i filen `p04_rapport.md` där ni övergripande beskriver
vem som gjorde vad, vilka testnivåer och verktyg ni använde, samt eventuella avgränsningar
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
* Git-historiken visar att du personligen har skrivit fungerande enhets- och/eller
  komponenttester för minst en del av kodbasen (en driver eller en ML-algoritm).
* Du kan förklara dina egna tester: vad de testar, vilken testnivå de tillhör
  (enhets-, komponent- eller integrationstest) och varför de är utformade som de är.
* Du kan redogöra för hur testsviten byggs och körs, både lokalt och i CI-pipelinen.
* Gruppens gemensamma testsvit och CI-pipeline uppfyller G-kraven i [Krav](#krav); annars
  finns inget konkret för dig att visa upp och förklara.

### VG (individuellt)
Utöver G-kraven ska du med stor skicklighet och hög precision kunna visa eget bidrag till minst
ett av kursplanens två VG-moment:
* **Säker och robust programvara:** du har själv identifierat och testat relevanta edge cases
  (ogiltig indata, gränsvärden, samtidiga händelser) och/eller åtgärdat fel som hittats av
  statisk analys eller sanitizers, och kan motivera varför just dessa gör systemet mer robust.
* **Ett testramverk för kontinuerlig testning:** du har själv byggt eller vidareutvecklat en
  väsentlig del av CI-pipelinen, teststrukturen eller ESP-IDF-mockningen, och kan förklara hur
  den möjliggör fortsatt testning genom produktens livscykel, inte bara att den finns, utan
  varför den är utformad som den är.

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
Besvara följande frågor gemensamt i `p04_rapport.md`, tillsammans med bidragsrapporten:
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
