# L05 - Lektionsanteckningar, demot
Till skillnad från **L02-L04** finns här inget lösningsförslag. Övningarna i
[bilaga B](../appendix/b_exercises.md) görs i ert eget repo och gås inte igenom i helklass, av den
enkla anledningen att varje grupps kodbas ser olika ut.

Det här är i stället en sammanfattning av **demot**: det repo som skrevs fram live under
lektionen, med en pipeline som växte fram commit för commit.

Demorepot ligger på GitHub: [Yrgo-25/ci-demo](https://github.com/Yrgo-25/ci-demo).

---

## Demorepot

```text
ci-demo/
├── .clang-format               - Delad formatteringskonfiguration, samma som i det här repot.
├── .github/workflows/ci.yml    - Pipelinen: två jobb.
├── .gitignore                  - Ignorerar den byggda binären "firmware".
├── .gitmodules                 - Testramverket yrgo-test som submodul i libs/test.
├── Makefile                    - build, run, clean, check-format, format.
└── main.cpp                    - "Firmwaren": ett Hello World-program på 15 rader.
```

Att projektet är så litet är hela poängen: när firmwaren består av en enda `printf()` finns det
inget annat än pipelinen som kan gå sönder, och därmed inget annat som kan stjäla uppmärksamheten
från det som lektionen faktiskt handlar om. Er egen kodbas är betydligt större - men som ni ser i
bilaga A är det bara *byggsteget* som skiljer.

---

## Så växte pipelinen fram, commit för commit
Historiken är avsiktligt uppdelad i små steg. Vill ni spela upp lektionen i efterhand går det att
läsa den rakt igenom med `git log --reverse --oneline` och `git show <commit>`:

| Commit | Vad som hände |
|---|---|
| `Add test framework` | `yrgo-test` läggs till som submodul i `libs/test`. Den används inte av pipelinen ännu - den ligger där för att testjobbet i **L06** ska ha någonstans att landa. |
| `Add .gitignore` | Den byggda binären ska inte checkas in. Det är just därför artefakter finns: en binär tar man ut ur en körning, man committar den inte. |
| `Add firmware` | `main.cpp` samt en `Makefile` med `build`, `run` och `clean`. |
| `Build firmware in CI` | Första `ci.yml`: 18 rader, ett enda jobb, två steg - checkout och `make build`. |
| `Run firmware in CI` | Ett steg till: `make run`. Att något *byggde* är inte samma sak som att det *fungerar*. |
| `Upload artifacts to CI` | Binären samlas i `artifacts/` och laddas upp med `upload-artifact`. |
| `Add .clang-format` | Den delade formatteringskonfigurationen, 88 rader, identisk med den i det här repot. |
| `Run clang-format in CI` | `check-format` och `format` läggs till i `Makefile`, och pipelinen får sitt andra jobb. |
| `Format code` | Första röda krysset, se nedan. |
| `Update ci.yml` | `needs: analyze-format` samt `submodules: recursive` läggs till. |

Ordningen är i sig en poäng: varje steg är litet nog att man ser exakt vad det gör, och exakt vad
som går sönder om det utelämnas. Bygg er egen pipeline på samma sätt, ett jobb och ett steg i
taget, i stället för att klistra in en färdig fil och undra varför den är röd.

De tio commitarna ovan är repot *som det såg ut när lektionen slutade*. Därefter har det byggts ut
med en timer-driver, enhetstester samt CI-skript, som ett exempel på hur en kodbas kan struktureras:
ett interface med en implementation bakom sig, tester i en egen mapp med en egen makefile, och de
kommandon pipelinen kör samlade i skript. Titta därför i historiken snarare än på antalet commits:
det är stegen fram till `Update ci.yml` som hör till **L05**.

---

## Den färdiga pipelinen
Så här såg `ci.yml` ut när lektionen var slut:

```yaml
name: CI

# Specify when to run the pipeline.
on:
  push:
  pull_request:

# Specify jobs to run.
jobs:
  firmware-build:
    name: Build ESP32 firmware
    runs-on: ubuntu-latest
    needs: analyze-format
    steps:
      - name: Checkout repository
        uses: actions/checkout@v5
        with:
          submodules: recursive
      - name: Build firmware
        run: make build
      - name: Run firmware
        run: make run
      - name: Collect artifacts in artifacts/
        run: |
          mkdir -p artifacts
          mv firmware artifacts
      - name: Upload artifacts
        uses: actions/upload-artifact@v7
        with:
          path: artifacts
          name: firmware
          if-no-files-found: error
          retention-days: 3

  analyze-format:
    name: Check format with clang-format
    runs-on: ubuntu-latest
    steps:
      - name: Checkout repository
        uses: actions/checkout@v5
      - name: Run analysis
        run: make check-format
```

Rad för rad förklaras varje nyckel i [bilaga A](../appendix/a_ci_startup.md). Notera bara två
saker här: `analyze-format` står *sist* i filen men körs *först*, eftersom ordningen mellan jobb
styrs av `needs:` och inte av var i filen de råkar stå - och att båda jobben har ett eget
checkout-steg, eftersom de kör på var sin egen maskin.

---

## Makefile-målen
| Kommando | Gör |
|---|---|
| `make` | Bygger och kör firmwaren (`build` följt av `run`). |
| `make build` | Bygger enbart. Samma kommando som CI:s byggsteg. |
| `make run` | Kör den byggda binären. |
| `make clean` | Tar bort binären. |
| `make check-format` | `clang-format --dry-run --Werror` på repots `.cpp`- och `.h`-filer. Ändrar ingenting. |
| `make format` | `clang-format -i` på samma filer, dvs. skriver ändringarna till filerna. |

```text
$ make
Hello, I'm your firmware!
```

Att kommandona bor i `Makefile` och inte direkt i pipelinen är det som gör ett rött jobb
felsökbart: `make check-format` på er egen dator ger exakt samma utskrift som jobbet i CI.

---

## Det första röda krysset
Direkt efter att formatteringskontrollen lagts till blev pipelinen röd - och felet var inget någon
hade upptäckt vid en genomläsning:

```text
./main.cpp:8:3: error: code should be clang-formatted [-Wclang-format-violations]
 * 
  ^
make: *** [Makefile:30: check-format] Error 1
```

Ett blanksteg i slutet av en rad i en doc-kommentar. Osynligt i editorn, meningslöst att bråka om
på en code review, och exakt den sortens sak ett verktyg ska sköta åt er. Commiten `Format code`
är resultatet av `make format`: ett enda tecken bort, och pipelinen grön igen.

Det andra som demonstrerades live syns inte i historiken alls: ett jobb vars `runs-on:` pekar på
en etikett som ingen registrerad runner har. Det blir varken grönt eller rött utan står kvar i
**`Queued`**, utan felmeddelande. Känn igen symptomet - det är alltid `runs-on:` ni ska titta på
först. Se avsnittet om `runs-on:` i [bilaga A](../appendix/a_ci_startup.md).

---

## En detalj värd att känna till: vad kontrollen faktiskt omfattar
`check-format`-målet plockar filer med `find . -name '*.cpp'` och `find . -name '*.h'`, dvs. allt
i repot. Vilka filer det blir beror på om submodulen är utcheckad eller inte:

| Var | Antal filer | Vilka |
|---|---|---|
| Lokalt, med submodulen hämtad | 9 | `main.cpp` samt de åtta filerna i `libs/test`. |
| I `analyze-format`-jobbet | 1 | Enbart `main.cpp` - jobbet checkar *inte* ut submoduler. |

Skillnaden är ofarlig här, eftersom `libs/test` har en egen `.clang-format` och redan är korrekt
formaterad. Men principen är värd att ta med sig till ert eget repo: ett formatteringsfel inne i en
submodul hade gjort `make check-format` rött *lokalt* men grönt *i CI*. Ligger er egen kod i en
submodul behöver också formatteringsjobbet `submodules: recursive` - och omvänt är det sällan ni
vill att er pipeline ska granska formatteringen i någon annans kod.

---

## Att köra demot själv

```bash
git clone --recursive https://github.com/Yrgo-25/ci-demo.git
cd ci-demo
make                # Bygger och kör firmwaren.
make check-format   # Kontrollerar formatteringen, ändrar inget.
```

Glömmer ni `--recursive` blir `libs/test` tom. Hämta då submodulen i efterhand:

```bash
git submodule update --init --recursive
```

---

## Demot kontra mallen i bilaga B
Demot skrevs fram live och är därför inte tecken för tecken identiskt med mallfilen i
[ci_templates](../appendix/ci_templates/.github/workflows/ci.yml). Det är **mallen** ni ska kopiera
in i ert eget repo; den här tabellen finns bara så att ni inte ska bli förvirrade när ni jämför:

| | Mallen | Demot |
|---|---|---|
| `on:` | Endast `main` (`branches: [main]`) | Alla branches |
| Formatteringsjobbets id | `check-format` | `analyze-format` |
| Firmware-jobbets namn | `Build firmware` | `Build ESP32 firmware` |
| Artefaktkatalog | `fw` | `artifacts` |
| `retention-days` | 1 | 3 |
| Extra steg | - | `Run firmware` (`make run`) |

Actionversionerna är däremot desamma i båda: `actions/checkout@v5` och
`actions/upload-artifact@v7`. Kontrollera ändå alltid vilka versioner som är aktuella när ni
sätter upp er egen pipeline, i stället för att lita på att en mall är färsk - `@v4` av båda dessa
actions är utfasad, eftersom den bygger på en Node.js-version som inte längre stöds.

---

## Vad ni gör härnäst
Övningarna i [bilaga B](../appendix/b_exercises.md) görs i **ert eget repo**, som en del av
**P04**: kopiera in `.clang-format` och `ci/`, få de två jobben gröna, och provocera fram ett rött
kryss per jobb för att se att pipelinen faktiskt fångar fel.

Förbered också det som **L06** bygger vidare på: testramverket i repot (gärna som submodul, precis
som i demot) samt ett kommando som bygger och kör er ännu tomma testsvit. Då är det bara att lägga
till ett tredje jobb när de första egna testerna finns.

---
