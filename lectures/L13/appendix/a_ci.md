# Bilaga A - Kontinuerlig integration med GitHub Actions

## Varför automatisera testkörning?
Ett test som bara körs manuellt, ibland, av den som råkar komma ihåg det, skyddar bara mot
regressioner den dagen någon faktiskt kör det. **Kontinuerlig integration (CI)** innebär att
testsviten (samt annan kontroll, t.ex. kodformattering) körs automatiskt vid varje push och
pull request, så att ett misslyckat test upptäcks direkt, av alla, varje gång.

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

## Att bygga ESP-IDF-firmware i CI
Utöver att bygga och köra er testsvit ska er egen pipeline i **P04** även bygga er firmware för
`ESP32-S3` via ESP-IDF, som ett eget jobb. Det är fullt möjligt på en vanlig, GitHub-hostad
runner utan fysisk hårdvara: att **kompilera** firmware kräver bara rätt verktygskedja, inte ett
riktigt kort, det är först **flashning** och körning mot riktig hårdvara som kräver det (se
**L17**).

I stället för att installera hela ESP-IDF-verktygskedjan för hand i varje körning kan ni
använda Espressifs officiella GitHub Action, som paketerar verktygskedjan i en Docker-image.
Ett ungefärligt exempel (kontrollera aktuell dokumentation för actionen för exakt syntax och
giltiga versioner):

```yaml
build-firmware:
  name: Build firmware
  runs-on: ubuntu-latest
  steps:
    - name: Check out repository
      uses: actions/checkout@v4
    - name: Build firmware with ESP-IDF
      uses: espressif/esp-idf-ci-action@v1
      with:
        esp_idf_version: v5.2
        target: esp32s3
        command: idf.py build
```

Jobbet verifierar alltså att firmwaren går att bygga med rätt verktygskedja. Ingen fysisk `ESP32-S3` behövs, eftersom kompileringen sker helt på GitHub Actions-runnern. Det är först vid flashning och körning på riktig hårdvara som ett faktiskt kort krävs.

En lyckad pipeline ger alltså förtroende för att projektet fortfarande följer kodstandarden, går att bygga och klarar sina tester.

---
