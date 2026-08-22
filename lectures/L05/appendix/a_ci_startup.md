# Bilaga A - CI-uppstart

## Varför redan nu?
**P04** kräver att allt arbete sker via branches och pull requests. Då finns det ingen anledning
att vänta med att automatisera kontrollerna: **kontinuerlig integration (CI)** innebär att bygget
och kodgranskningen körs automatiskt vid varje push och pull request av en maskin, varje gång, i
stället för av den som råkar komma ihåg det.

Nyttan är störst tidigt. En formatteringskontroll som funnits sedan **L05** slipper ni någonsin
städa upp efter i efterhand, och ett rött kryss på en pull request hindrar ej fungerande kod från att nå
`main` under hela projektets gång, inte bara under de sista veckorna.

Den här bilagan räcker för att få en fungerande pipeline på plats. Hur den fungerar i detalj, och
hur den härdas vidare, går vi igenom i **L13**.

---

## Utgångspunkten: ett minimalt projekt
Demot på lektionen använder ett så enkelt projekt som möjligt så att inget annat än pipelinen
ska stå i vägen. En enda källkodsfil, som får representera "firmware":

```cpp
/**
 * @brief Firmware demo.
 */
#include <cstdio>

int main()
{
    std::printf("Hello! I'm your firmware!\n");
    return 0;
}
```

...och en `Makefile` som bygger den, samt kapslar in formatteringskontrollen:

```makefile
# Application target (the firmware itself).
TARGET := firmware

# C++ compiler.
CXX_COMPILER := g++

# C++ compiler flags.
CXX_FLAGS := -Wall -Werror -std=c++17 -Iinclude

# Source files.
SRC_FILES := source/main.cpp

# Build the firmware.
build:
	@$(CXX_COMPILER) $(SRC_FILES) -o $(TARGET) $(CXX_FLAGS)

# Analyze format.
check-format:
	@clang-format --dry-run --Werror $$(find . -name '*.hpp') $$(find . -name '*.cpp')
```

Poängen med att lägga kommandona i `Makefile` i stället för direkt i pipelinen är att ni kör
**exakt samma kommandon lokalt som CI kör**. Ett rött jobb går alltid att återskapa på den egna
datorn med ett enda kommando, och pipelinen blir kort nog att läsa i ett svep.

---

## Pipelinen, rad för rad
Filen skrivs fram live på lektionen, en rad i taget. Det här avsnittet förklarar vad varje rad
betyder, så att ni kan följa med i genomgången i stället för att hinna skriva av. Den färdiga
filen finns som mall i [ci_templates](./ci_templates/.github/workflows/ci.yml).

En workflow-fil ligger alltid i `.github/workflows/` i repots rot och är skriven i YAML, där
**indraget är betydelsebärande**: det är indraget, inte klamrar, som avgör vad som hör ihop med
vad. Ett felaktigt indrag är den vanligaste orsaken till att en workflow inte startar alls.

### Vad pipelinen heter, och när den körs

```yaml
name: CI

on:
  push:
    branches: [main]
  pull_request:
    branches: [main]
```

* **`name:`** är namnet som visas i fliken *Actions* på GitHub. Rent kosmetiskt, men trevligt när
  ni senare har mer än en workflow.
* **`on:`** talar om *vad som utlöser* pipelinen. Utan den här nyckeln körs den aldrig.
* **`push:`** betyder "kör vid varje push", och **`pull_request:`** "kör när en pull request
  öppnas eller uppdateras".
* **`branches: [main]`** begränsar båda till `main`. För `push` betyder det pushar direkt till
  `main`; för `pull_request` betyder det pull requests *riktade mot* `main`, alltså exakt det
  arbetsflöde **P04** kräver. Utelämnas raden körs pipelinen för alla branches.

### Jobben

```yaml
jobs:
  firmware-build:
    name: Build firmware
    runs-on: ubuntu-latest
    needs: check-format
```

* **`jobs:`** samlar pipelinens jobb. Varje jobb kör på en egen, ren maskin.
* **`firmware-build:`** är jobbets **id**, alltså namnet ni själva väljer. Det är det här namnet
  andra rader refererar till (`needs:`), och det som dyker upp när ni konfigurerar branch
  protection i **L13**. Inga mellanslag.
* **`name:`** är den läsbara etikett som visas i gränssnittet. Sätts den inte används id:t.
* **`runs-on:`** väljer vilken sorts maskin jobbet ska köra på, se avsnittet om `runs-on:` längre
  ned.
* **`needs:`** styr ordningen mellan jobb, se nästa avsnitt.

### Stegen i ett jobb

```yaml
    steps:
      - name: Checkout repository
        uses: actions/checkout@v4
        with:
          submodules: recursive
      - name: Build firmware
        run: make build
```

* **`steps:`** är en **lista** (varje `-` inleder ett nytt steg) som körs **uppifrån och ned**, i
  ordning, på samma maskin. Misslyckas ett steg avbryts jobbet där.
* **`name:`** på ett steg är rubriken det får i loggen. Bra namn gör en misslyckad körning
  betydligt snabbare att felsöka.
* **`uses:`** hämtar in en färdigskriven, återanvändbar *action*. `actions/checkout@v4` hämtar hem
  koden från ert repo; `@v4` är versionen. **Utan checkout-steget står jobbet på en tom maskin
  utan er kod**, vilket är ett vanligt nybörjarfel.
* **`with:`** skickar in parametrar till den action som `uses:` pekar ut. Här betyder
  `submodules: recursive` att även submoduler hämtas, vilket behövs om testramverket ligger som
  submodul.
* **`run:`** kör i stället ett vanligt skalkommando på maskinen, alltså exakt samma `make`-mål som
  ni kör själva. Det är därför kommandona bor i `Makefile`: ett rött jobb går att återskapa lokalt.

Ett steg använder antingen `uses:` eller `run:`, aldrig båda.

### Flera kommandon i ett steg

```yaml
      - name: Collect artifacts
        run: |
          mkdir -p fw
          mv firmware fw
```

* **`run: |`** där `|` betyder "allt indraget under den här raden är text, rad för rad". Så kör ni
  flera kommandon i ett och samma steg, i stället för att skriva ett steg per kommando.
* Stegen samlar det som ska sparas i katalogen `fw`, se avsnittet om artefakter nedan.

### Det andra jobbet

```yaml
  check-format:
    name: Analyze code with clang-format
    runs-on: ubuntu-latest
    steps:
      - name: Checkout repository
        uses: actions/checkout@v4
      - name: Perform analysis
        run: make check-format
```

Samma byggstenar igen, men enklare: inget `needs:` (det här jobbet väntar inte på något), inget
`with:` (ingen submodul behövs för att läsa källkoden) och bara två steg. Notera att också det här
jobbet behöver sitt eget checkout-steg, eftersom det körs på en **egen** maskin som inte vet något
om vad det andra jobbet gjort.

Kontrollera aktuella versioner av de actions ni använder, de uppdateras med tiden.

---

## Ordningen mellan jobben: `needs:`
Jobb körs som standard **parallellt och oberoende** av varandra. Raden `needs: check-format` i
`firmware-build` ändrar på det: firmware-bygget startar inte förrän formatteringskontrollen har
lyckats.

Det är ett medvetet val, med en tydlig avvägning:

| | Parallellt (utan `needs:`) | I ordning (med `needs:`) |
|---|---|---|
| Tid till besked | Båda svaren efter en körning | Bygget får vänta på kontrollen |
| Vid formatteringsfel | Bygget körs ändå | Bygget körs inte alls, ingen tid slösas |
| Vid två samtidiga fel | Ni ser båda direkt | Ni ser det andra först efter nästa push |

`needs:` passar bra när det första jobbet är *snabbt och billigt* och det andra är *långsamt och
dyrt*, vilket stämmer här: `clang-format` tar sekunder, ett riktigt firmware-bygge tar minuter.
Är båda jobben snabba är parallell körning oftast att föredra. Vi återkommer till avvägningen i
**L13**.

---

## Att samla in och ladda upp artefakten
En runner raderas när jobbet är klart, och med den allt jobbet byggde. Vill ni behålla något, t.ex.
den `firmware`-binär som bygget nyss producerade, laddar ni upp den som en **artefakt**. Den dyker
då upp som en nedladdningsbar fil på körningens sida på GitHub, under
*Actions → (körningen) → Artifacts*.

Uppladdningen är det sista steget i `firmware-build`:

```yaml
      - name: Upload artifacts
        uses: actions/upload-artifact@v4
        with:
          path: fw
          name: firmware
          if-no-files-found: error
          retention-days: 1
```

* **`uses: actions/upload-artifact@v4`** är den färdiga action som sköter uppladdningen. Allt
  under `with:` är parametrar till den.
* **`path:`** pekar ut vad som ska laddas upp, här katalogen `fw` som insamlingssteget
  (`mkdir -p fw`, `mv firmware fw`) nyss fyllde. Att först samla ihop filerna i en egen katalog gör
  det lätt att lägga till fler senare, t.ex. en `.map`-fil eller byggloggar, utan att röra resten
  av pipelinen.
* **`name:`** är namnet artefakten får i gränssnittet, alltså det ni klickar på för att ladda ner.
* **`if-no-files-found: error`** gör att jobbet blir **rött** om `path:` inte matchade något. Utan
  den skulle en felstavad sökväg ge en grön körning med en tom artefakt, vilket är precis den
  sortens tysta fel ni inte vill ha i en pipeline.
* **`retention-days:`** styr hur länge artefakten sparas (utan angivelse gäller repots standard,
  som regel 90 dagar). Sätt den lågt: en binär per körning fyller snabbt upp lagringsutrymmet utan
  att ni har nytta av gamla byggen.

Att uppladdningen är ett **steg i samma jobb**, och inte ett eget jobb, är inget godtyckligt val.
Varje jobb får en egen, ren runner, och den runner som kör `check-format` har aldrig sett någon
byggd binär. Ett eget "upload"-jobb skulle alltså inte hitta något att ladda upp: **filer laddas
upp från det jobb som skapade dem.** Det är också därför artefakter finns till att börja med, de är
sättet att få ut något ur ett jobb innan runnern försvinner.

---

## Kodformattering med `clang-format`
`clang-format` gör formatteringen till något ingen behöver diskutera på en code review. Verktyget läser en
`.clang-format`-fil i repots rot och formaterar om koden efter den, deterministiskt och likadant
för alla i gruppen.

De två kommandon ni behöver:

```bash
clang-format --dry-run --Werror src/min_fil.cpp   # Visa vad som skulle ändras, ändra inget.
clang-format -i src/min_fil.cpp                   # Skriv ändringarna till filen.
```

För hela kodbasen finns [ci/format.sh](../../../ci/format.sh), som kör `clang-format` på all
C/C++-kod och `black` på alla Python-filer:

```bash
ci/format.sh          # Formatera alla filer.
ci/format.sh --check  # Kontrollera utan att ändra, används i CI.
```

Poängen med `--check` är att den inte ändrar någonting, bara rapporterar. Det är därför det är
just den varianten som körs i `check-format`-jobbet ovan: pipelinen ska tala om att formatteringen
glidit isär, inte tyst skriva om er kod åt er.

---

## `runs-on:` - var jobbet faktiskt körs
`runs-on: ubuntu-latest` betyder att GitHub startar en färsk, tillfällig virtuell maskin åt er,
kostnadsfritt för publika repon. **Det är det värdet ni ska använda i P04**, och det demot utgår
från.

Det viktiga att förstå är att `runs-on:` inte är ett *kommando* utan ett **krav**: raden beskriver
vilken sorts maskin jobbet behöver, och GitHub letar sedan upp en ledig maskin som matchar. Vem som
tillhandahåller maskinen är en separat fråga.

### Self-hosted runner: en egen maskin med en egen etikett
Man kan också registrera sin **egen** dator som runner (*Settings → Actions → Runners → New
self-hosted runner*, följt av det utdelade konfigurationsskriptet) och ge den en valfri etikett,
t.ex. `yrgo-ubuntu`. Jobbet begär då den etiketten i stället:

```yaml
    runs-on: yrgo-ubuntu
```

Etiketter är alltså inget annat än fritextnamn ni själva hittar på. Allt annat i pipelinen är
identiskt, det är bara den här raden som skiljer. Två skäl gör det intressant: verktygskedjor kan
förinstalleras på en egen maskin i stället för att hämtas varje körning, och en egen maskin kan ha
**hårdvara inkopplad**. Det sistnämnda är vad som gör automatiserad hårdvarutestning möjlig, se
**L17**.

### Vad som händer när ingen maskin matchar
Eftersom `runs-on:` är ett krav, och inte en order, händer något som förvånar många första gången:
om ingen registrerad runner har den begärda etiketten, t.ex. för att maskinen är avstängd eller
borttagen, blir jobbet **varken grönt eller rött**. Det blir stående i **`Queued`**, i väntan på en
maskin som aldrig svarar. Inget felmeddelande, inget avbrott.

Det demonstreras på lektionen, så att ni känner igen symptomet. Två praktiska följder:
* Ser ni ett jobb som fastnat i *Queued* är `runs-on:` det första ni ska kontrollera.
* Använd `ubuntu-latest` i era egna repon, om ni inte faktiskt har registrerat en egen runner. En
  avskriven etikett från demot ger exakt det här symptomet.

---

## För er egen kodbas: att bygga riktig firmware
Binären ovan är ett helt vanligt C++-program, byggt med värddatorns kompilator. För **P04** ska
samma jobb i stället bygga er **firmware** för `ESP32-S3`. Det kräver ingen fysisk hårdvara: att
**kompilera** firmware behöver bara rätt verktygskedja, det är först **flashning** och körning mot
ett riktigt kort som kräver hårdvara (se **L17**).

I stället för att installera hela ESP-IDF-verktygskedjan för hand i varje körning finns Espressifs
officiella GitHub Action, som paketerar verktygskedjan i en Docker-image. Byt ut byggsteget mot
den, så bygger jobbet er firmware i stället:

```yaml
      - name: Build firmware with ESP-IDF
        uses: espressif/esp-idf-ci-action@v1
        with:
          esp_idf_version: v5.2
          target: esp32s3
          command: idf.py build
          # Lägg till 'path:' här om ESP-IDF-projektet inte ligger i repots rot.
```

Insamlingssteget pekar då på `build/*.bin` i stället för på `firmware`. Notera hur lite som
faktiskt ändras: checkout, uppladdning, `needs:` och jobbens struktur är identiska, det är bara
*byggsteget* som byter verktygskedja. Pipelinens form är oberoende av vad som byggs.

Kontrollera aktuell dokumentation för actionen för exakt syntax och giltiga versioner. Ligger inte
ert ESP-IDF-projekt i repots rot måste ni peka ut var det ligger via actionens `path`-parameter.
Det är den vanligaste orsaken till att jobbet misslyckas första gången.

Den nedladdade `.bin`-filen ger er tre konkreta saker:
* En gruppmedlem som inte har ESP-IDF installerat kan ändå få tag på en byggd firmware.
* Ni kan flasha exakt den binär som byggdes från en viss commit, i stället för att bygga om lokalt
  och hoppas att det blev samma sak.
* Den binären är utgångspunkten för HW/SW-integrationstesterna i **L17**.

---

## Och testsviten?
Demot ovan bygger och granskar kod, men kör inga tester. Det är avsiktligt: målet med demot är att
visa pipelinens delar med minsta möjliga projekt, och firmware-demot innehåller inga tester att
köra.

I ert eget repo ser det annorlunda ut. Där växer det fram en testsvit för era egna
`Esp32s3`-drivers och era ML-algoritmer (**P04**), och den är själva anledningen till att ni har en
pipeline över huvud taget. Ett test som bara körs när någon råkar komma ihåg det skyddar ingenting;
det är först när det körs vid varje push som det är en regressionsspärr. Lägg därför till ett
tredje jobb som bygger och kör er testsvit:

```yaml
  unit-tests:
    name: Build and run unit tests
    runs-on: ubuntu-latest
    steps:
      - name: Checkout repository
        uses: actions/checkout@v4
        with:
          submodules: recursive
      - name: Build and run test suite
        run: make test
```

Två praktiska förutsättningar innan jobbet kan bli grönt:
* **Testramverket måste finnas i ert repo.** Enklast som en submodul, precis som i det här repot
  ([yrgo-test](https://github.com/yrgo-libs/yrgo-test.git)). Det är därför checkout-steget använder
  `submodules: recursive`; utan det står jobbet med en tom katalog där ramverket skulle ha legat.
* **Det måste finnas ett kommando som bygger och kör sviten**, t.ex. ett `test`-mål i er
  `Makefile`. Samma princip som tidigare: pipelinen ska köra exakt det ni kör lokalt.

Observera att testerna ni skrivit under **L02–L04** ligger i kursens ATmega328p-övningsbibliotek,
inte i er egen kodbas. Det är alltså inte *de* testerna som ska köras här, utan era egna, för era
`ESP32-S3`-drivers och `ml::lin_reg::Adaptive`. Tekniken är densamma, koden är er.

Att enhetstesta en riktig `Esp32s3`-driver kräver mockning av ESP-IDF, vilket ni går igenom först i
**L06**. Vänta därför inte på att ha "rätt" tester innan ni lägger till jobbet: börja med något som
går att testa redan i dag, t.ex. `driver::tempsensor::Tmp36`s temperaturformel eller er adaptiva
regressionsmodell `ml::lin_reg::Adaptive` från **P03**, som båda är ren beräkningslogik utan
hårdvaruberoenden. Poängen nu är
att få kedjan på plats; driverstesterna fyller ni på med från **L06** och framåt.

---

## Vad som inte hör hemma i pipelinen
Allt som kräver fysisk hårdvara ligger utanför de GitHub-hostade runnerna: de har inget kort
inkopplat, och kan alltså varken flasha eller mäta något. Det betyder inte att det är omöjligt att
automatisera, bara att det kräver en runner som körs på en maskin med ett kort inkopplat. Den
varianten tittar vi på i **L17**.

---
