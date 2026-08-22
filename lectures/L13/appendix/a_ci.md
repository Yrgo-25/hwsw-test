# Bilaga A - Kontinuerlig integration med GitHub Actions

## Varför automatisera testkörning?
Ett test som bara körs manuellt, ibland, av den som råkar komma ihåg det, skyddar bara mot
regressioner den dagen någon faktiskt kör det. **Kontinuerlig integration (CI)** innebär att
testsviten (samt annan kontroll, t.ex. kodformattering) körs automatiskt vid varje push och
pull request, så att ett misslyckat test upptäcks direkt, av alla, varje gång.

Ni har redan en pipeline som gör det, den ni satte upp i **L05**. Den här lektionen handlar om
att förstå den i detalj och härda den, inte om att bygga en ny från grunden.

---

## Anatomin i en GitHub Actions-pipeline
Det här repot har en egen pipeline i [.github/workflows/ci.yml](../../../.github/workflows/ci.yml),
värd att läsa som ett konkret exempel:

* **`on:`** anger vad som utlöser pipelinen, här varje `push` och `pull request` mot `main`.
* **`jobs:`** innehåller en eller flera jobb som körs (som standard parallellt, oberoende av
  varandra). Repot har två: `build-and-test`, som bygger `yrgo::test` och kör båda testsviterna,
  samt `format-check`, som kör `clang-format` (se **L05**) mot all C/C++-kod och `black` mot alla
  Python-filer, i stället för mot enstaka filer för hand.
* Varje jobb har en lista av **`steps:`** som körs i ordning. Ett steg använder antingen
  * en färdig, återanvändbar åtgärd via `uses:` (t.ex. `actions/checkout@v4` för att hämta koden), eller
  * kör ett eget kommando via `run:` (t.ex. `bash ci/build.sh`).
* Om något steg i ett jobb misslyckas (returnerar en felkod som inte är noll), markeras hela
  pipelinen som misslyckad, vilket syns direkt i pull requesten på GitHub.

Det är värt att stanna vid att jobben körs **parallellt och oberoende**, inte i följd. Ett
formatteringsfel stoppar alltså inte testkörningen, och tvärtom: ni får reda på båda felen efter
en enda körning, i stället för att behöva rätta det ena för att ens få se det andra. Ett jobb är
också den enhet som får en egen, ren runner; därför hör "bygg och kör tester" och "kontrollera
formattering" hemma i var sitt jobb.

Er egen pipeline har samma uppdelning. Den fick sina två första jobb i **L05**, `firmware-build`
och `check-format`, körda *i ordning* via `needs:` i stället för parallellt (se
[bilaga A i L05](../../L05/appendix/a_ci_startup.md)), och ett `unit-tests`-jobb i **L06**, när de
första egna testerna fanns. Efter **L12** har den sannolikt också ett jobb för statisk analys eller
sanitizers. Principen är densamma oavsett antal: ett jobb, en fråga, en egen runner.

Er pipeline har alltså tre jobb: formatteringskontroll, firmware-bygge och testkörning. Frågan är
inte längre *om* de finns, utan hur de förhåller sig till varandra. Ett testjobb är
varken lika snabbt som formatteringskontrollen eller lika dyrt som ett fullt firmware-bygge, så
var det hör hemma i kedjan är ett verkligt designbeslut, inte en formalitet.

```text
                push / pull_request mot main
                            |
                     GitHub Actions
                            |
            +---------------+---------------+
            |                               |
      build-and-test                  format-check
      (checkout + submodul)           (checkout)
      bash ci/build.sh                bash ci/format.sh --check
            |                               |
            +---------------+---------------+
                            |
            Alla jobb gröna  ->  pipeline godkänd
            Något jobb rött  ->  hela pipelinen misslyckad
```

| Nyckelord | Betydelse | I det här repot |
|---|---|---|
| `on:` | Vad som utlöser pipelinen | `push` och `pull_request` mot `main` |
| `jobs:` | Oberoende jobb, körs parallellt som standard | `build-and-test`, `format-check` |
| `steps:` | Steg inom ett jobb, körs i ordning | checkout → installera → bygg/kontrollera |
| `uses:` | Färdig, återanvändbar action | `actions/checkout@v4` |
| `run:` | Eget kommando på runnern | `bash ci/build.sh` |

---

## Att härda pipelinen
En pipeline som fungerar är inte samma sak som en pipeline man litar på. Tre saker gör störst
skillnad när den väl används dagligen av en hel grupp.

### Branch protection: att ett rött kryss faktiskt betyder något
Som standard *visar* GitHub att jobben misslyckades, men hindrar ingen från att merga ändå. Under
*Settings → Branches → Add branch ruleset* för `main` kan ni kräva att namngivna jobb har lyckats
innan en pull request går att merga (*Require status checks to pass*). Först då är pipelinen en
grind och inte bara en lampa.

Det är också den inställning som gör kravet i **P04** meningsfullt: all utveckling sker via
branches och pull requests, och det är på pull requesten kontrollen ska ske.

### Caching: att inte bygga om samma sak varje gång
Varje jobb får en ren runner, vilket är poängen, men det betyder också att allt som hämtas eller
byggs hämtas och byggs om från noll varje körning. Det som är dyrt och sällan ändras kan sparas
mellan körningar med `actions/cache`, t.ex. ESP-IDF:s verktygskedja eller nedladdade beroenden.
Nyckeln är vad cachen ska brytas mot: cachar ni för brett riskerar ni att bygga vidare på ett
gammalt resultat och missa ett verkligt fel.

### `concurrency`: att inte köra körningar ni inte längre bryr er om
Pushar någon tre gånger i rad till samma branch startar tre körningar, varav de två första redan
är ointressanta. Med `concurrency` avbryts den föregående körningen automatiskt när en ny startar
för samma branch:

```yaml
concurrency:
  group: ${{ github.workflow }}-${{ github.ref }}
  cancel-in-progress: true
```

---

## Vad som inte hör hemma i pipelinen
GitHubs runner är en virtuell maskin i ett datacenter. Den har ingen `ESP32-S3` inkopplad, och
kan alltså varken flasha, mäta en blinkperiod eller läsa av en riktig temperatursensor. Den
gränsen är fysisk, inte teknisk: allt som bara kräver en verktygskedja går att automatisera, allt
som kräver ett kort gör det inte, i alla fall inte på en GitHub-hostad runner.

Det betyder inte att hårdvarutestning är dömd till att vara manuell för alltid. Kör ni en egen
runner på en maskin med ett kort inkopplat kan även flashning och test mot fysisk hårdvara
automatiseras. Den varianten, och dess praktiska baksidor, tittar vi på i **L17**.

En lyckad pipeline ger alltså förtroende för att projektet fortfarande följer kodstandarden, går
att bygga för både värddator och `ESP32-S3`, och klarar sina tester. Att det dessutom *fungerar*
på riktig hårdvara är en separat fråga.

---
