# Bilaga B - Övningsuppgifter

Övningarna nedan görs i **ert eget repo** och är en del av projektarbetet i **P04**. De gås inte
igenom i helklass efteråt, eftersom varje grupps kodbas ser olika ut; utgå i stället från demot i
[bilaga A](./a_ci_startup.md) och fråga under lektionen när ni kör fast.

---

## Kodövning: kodformattering med `clang-format`
Ni får färdiga mallfiler att utgå från, i [ci_templates](./ci_templates/) i den här bilagan:
[.clang-format](./ci_templates/.clang-format), [ci/format.sh](./ci_templates/ci/format.sh),
[ci/build.sh](./ci_templates/ci/build.sh) samt
[.github/workflows/ci.yml](./ci_templates/.github/workflows/ci.yml) (samma filer som används i
det här repot).
* Kopiera `.clang-format`, `ci/`-mappen samt `.github`-mappen till roten av er egen kodbas från
  **P02**. Kontrollera att `ci/format.sh` är körbar (`chmod +x ci/format.sh`).
* Kör `clang-format` mot en eller ett par av era egna filer och granska diffen. Ändra inte
  koden för hand innan ni gör detta, poängen är att se vad verktyget självt justerar:

  ```bash
  clang-format --dry-run --Werror src/min_fil.cpp   # visa vad som skulle ändras
  clang-format -i src/min_fil.cpp                   # skriv ändringarna till filen
  ```

  Använd inte `ci/format.sh` för det här steget: skriptet formaterar *alla* filer i repot på en
  gång, vilket gör diffen omöjlig att överblicka. Det är avsett för CI och för att formatera hela
  kodbasen när ni väl bestämt er.
* Diskutera i gruppen: höll ni redan er kod till konfigurationens regler, eller ändrades en hel
  del? Är det något i konfigurationen ni är oense om?

---

## Kodövning: en första CI-pipeline
Mallen [.github/workflows/ci.yml](./ci_templates/.github/workflows/ci.yml) innehåller de två
jobben från [bilaga A](./a_ci_startup.md): `check-format`, som granskar kodformatteringen, och
`firmware-build`, som bygger firmwaren och laddar upp den som artefakt. Målet med övningen är att
få båda gröna i ert eget repo.

* Se till att `make build` och `make check-format` fungerar lokalt i ert eget repo, det är de två
  kommandon pipelinen kör. Föredrar ni skript (`ci/build.sh`, `ci/format.sh --check`) går det lika
  bra; det viktiga är att CI kör exakt samma kommando som ni gör själva.
* Se över `submodules: recursive` i checkout-steget, det behövs bara om ni har testramverket som
  submodul.
* Byt ut byggsteget i `firmware-build` mot ESP-IDF-varianten (se bilaga A), så att jobbet bygger
  er egen firmware, och samla in `build/*.bin` i stället. Ligger ESP-IDF-projektet inte i repots
  rot måste ni peka ut det via actionens `path`-parameter, annars hittar den inget att bygga.
* Skapa en branch, pusha och öppna en pull request. Följ körningen under fliken *Actions* på
  GitHub. Titta särskilt på loggen för ett jobb som misslyckas: felmeddelandet där är detsamma
  som ni hade fått lokalt.
* Ladda ner artefakten från körningen (*Actions → körningen → Artifacts*) och kontrollera att
  filen faktiskt ligger där. Spara den, i **L17** är det en sådan binär ni utgår från vid
  HW/SW-integrationstestning.
* Provocera fram ett rött kryss, en gång per jobb, och verifiera att pipelinen faktiskt fångar
  det:
  * Ändra formatteringen i en fil för hand (lägg t.ex. till extra indrag) och pusha. Notera att
    `firmware-build` då inte ens startar, tack vare `needs:`.
  * Inför ett kompileringsfel medvetet och pusha.
  * Peka insamlingssteget på en fil som inte finns, och kontrollera att `if-no-files-found: error`
    gör jobbet rött i stället för att tyst ladda upp ingenting.
  * Återställ därefter allt och kontrollera att pipelinen blir grön igen.
* Diskutera i gruppen: vad hade hänt om de två jobben i stället varit steg i ett enda jobb? Och
  vad hade hänt om ni lagt uppladdningen i `check-format`-jobbet?

---

## Kodövning: låt era egna tester köras automatiskt
Pipelinen ovan bygger och granskar kod. Lägg nu till jobbet som gör den till en testpipeline, se
bilaga A:
* Lägg till testramverket i ert repo, som submodul, om ni inte redan gjort det.
* Lägg till ett kommando som bygger och kör er testsvit (t.ex. `make test`), och kontrollera att
  det fungerar lokalt.
* Lägg till ett `unit-tests`-jobb som kör det kommandot.
* Låt ett testfall misslyckas medvetet (t.ex. `EXPECT_EQ(1, 2)`), pusha, och verifiera att
  pipelinen blir röd och att felet syns i jobbets logg. Återställ därefter.

Det handlar alltså om **era egna** tester, för era `ESP32-S3`-drivers och ML-algoritmer i **P04**,
inte om testerna ni skrivit mot ATmega328p-övningsbiblioteket under **L02–L04**. Har ni ännu inte
skrivit några egna tester går det bra att börja med ett enda, mot något som inte kräver mockning av
ESP-IDF (det kommer i **L06**), t.ex. `driver::tempsensor::Tmp36`s temperaturformel eller
`ml::lin_reg::Adaptive` från **P03**. Poängen är att få kedjan på plats; testerna fyller ni på med löpande
under kursen.

---
