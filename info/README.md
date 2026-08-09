# Kursinfo

## Lärare
Erik Pihl ([erik.axel.pihl@gmail.com](mailto:erik.axel.pihl@gmail.com))

---

## Kursplan - Mjuk- och hårdvarutestning

| Vecka | Datum | Föreläsning | Ämne |
|-------|-------|-------------|------|
| 34 | 19/8 | L01 | Kursintroduktion, teststrategi och testpyramiden |
| 34 | 20/8 | L02 | Enhetstester (del I): grunder samt GPIO-drivern, introduktion till **P04** |
| 35 | 26/8 | L03 | Enhetstester (del II): den seriella drivern, arbete med **P04** |
| 36 | 2/9 | L04 | Enhetstester (del III): timer-drivern, arbete med **P04** |
| 37 | 9/9 | L05 | Testbar design: interfaces, dependency injection samt `clang-format`, arbete med **P04** |
| 38 | 16/9 | L06 | Stubbar och virtuell hårdvara, arbete med **P04** |
| 38 | 17/9 | L07 | Enhetstester (del IV): `Tmp36`s temperaturformel samt ML-algoritmerna, arbete med **P04** |
| 39 | 23/9 | L08 | Komponenttester (del I): systemlogik och stubbar, arbete med **P04** |
| 40 | 30/9 | L09 | Komponenttester (del II): gränsvärden och edge cases, arbete med **P04** |
| 41 | 7/10 | L10 | Praktiskt prov 1 (gemensamt med Maskininlärning) |
| 2 | - | L11 | Komponenttester (del III): täckningsgrad och testkvalitet, arbete med **P04** |
| 2 | - | L12 | Statisk analys och verktyg för korrekthet, arbete med **P04** |
| 3 | - | L13 | Testautomatisering och kontinuerlig integration (CI), arbete med **P04** |
| 3 | - | L14 | Mätdatainsamling (del I): insamling från DUT, arbete med **P04** |
| 4 | - | L15 | Mätdatainsamling (del II): processning och strukturerad lagring, arbete med **P04** |
| 4 | - | L16 | Visualisering av mätdata och jämförelse mot kravställning, arbete med **P04** |
| 5 | - | L17 | HW/SW-integrationstestning samt säker och robust mjukvara, arbete med **P04** |
| 5 | - | L18 | Projektarbete (del I): **P04** |
| 6 | - | L19 | Projektarbete (del II) och förberedelse inför det praktiska provet |
| 6 | - | L20 | Praktiskt prov 2 (gemensamt med Maskininlärning) och kursavslut |

**OBS!** Kursen är vilande från vecka 42 fram till årsskiftet; L11 återupptas i januari. Veckor
för L11–L20 är preliminära och kan komma att justeras. Datum för L10 respektive L20 styrs av när
de gemensamma proven genomförs (se [Praktiska prov](#praktiska-prov)).

---

## Examination och betygsnivåer

### Upplägg
* Ett projekt (**P04**).
* Två praktiska prov, gemensamma med kursen Maskininlärning.

### Praktiska prov
Båda proven är gemensamma med kursen [Maskininlärning](https://github.com/Yrgo-25/Machine-Learning)
och genomförs under den här kursens **L10** respektive **L20**, eftersom dessa lektioner
infaller senare i schemat än motsvarande lektioner i Maskininlärning. Provet består av en
gemensam uppgiftsuppsättning per tillfälle: en delmängd av uppgifterna examinerar
maskininlärning, en annan delmängd examinerar testning. Betyg sätts separat per kurs, utifrån
den delmängd av uppgifterna som hör till respektive kurs.

Denna kurss del av respektive prov täcker:
* **Prov 1:** Enhets- och komponenttester av ett neuralt nätverks dense-lager, skrivna med
  `yrgo::test`. Testteori kring skillnaden mellan enhets- och komponenttester.
* **Prov 2:** Enhets- och komponenttester av konvolutionella lager (Conv, MaxPool, Flatten)
  samt ett helt CNN uppbyggt via stubbar. Testteori kring varför stubbar används i
  komponenttester.

Övningstentorna, inklusive den kod ni ska utgå från, delas mellan kurserna och finns i
Maskininlärning-kursens repo:
* [Övningstentamen 1](https://github.com/Yrgo-25/Machine-Learning/blob/main/exams/exam1/practice_exam1.md): uppgift 8–10 examinerar testning. Gås igenom i Maskininlärning-kursens **L10**.
* [Övningstentamen 2](https://github.com/Yrgo-25/Machine-Learning/blob/main/exams/exam2/practice_exam2.md): uppgift 6, 10–11 examinerar testning. Gås igenom i Maskininlärning-kursens **L20**.

### Poängfördelning
* **P04** ger upp till 4 poäng per student (**G** = 2p, **VG** = 4p), satta individuellt enligt
  kriterierna i [projektbeskrivningen](../projects/P04/README.md#bedömning).
* **Praktiskt prov 1** ger upp till 2 poäng (**G** = 1p, **VG** = 2p), baserat på
  testuppgifterna i övningstentamen 1.
* **Praktiskt prov 2** ger upp till 2 poäng (**G** = 1p, **VG** = 2p), baserat på
  testuppgifterna i övningstentamen 2.

### Betygsnivåer
Totalt 8 poäng:
* 4 <= **G** < 6 poäng
* 6 <= **VG** <= 8 poäng

För godkänt ska samtliga examinerande moment vara godkända.

---

## Kursmaterial

### Litteratur
* Utgörs av textdokument som bifogas och/eller länkas till i samband med lektionerna de behandlas.

### Hårdvara
Följande hårdvara, som finns på skolan, kommer att användas under kursen:
* **ESP32-S3** För **P04** samt HW/SW-integrationstestning.
* Multimeter samt referenstermometer: för de manuella integrationstesterna i **L17** (VG), där
  ett mätvärde från systemet ska jämföras mot en oberoende mätning.

### Mjukvara
* [Visual Studio Code](https://code.visualstudio.com/download):
    * Primär editor, samma som i tidigare kurser.
* ESP-IDF:
    * För kompilering och flashning av **ESP32-S3**.
    * Installerades under *Hårdvarunära programmering C/C++*.
* `yrgo::test`:
    * Kursens C++17-testramverk, se [libs/test](../libs/test/README.md).
* `clang-format`:
    * Automatisk kodformattering, se [ci/format.sh](../ci/format.sh). Introduceras i **L05** och
      kontrolleras i CI.
* `clang-tidy` samt `cppcheck`:
    * Statisk analys av C/C++-kod.
* `gcov` samt `lcov`:
    * Mätning av täckningsgrad för testsviten, se **L11**.
* AddressSanitizer/UndefinedBehaviorSanitizer (ASan/UBSan):
    * Körtidsverktyg för att hitta minnesfel och odefinierat beteende.
* Python 3 med `matplotlib` samt `pyserial`:
    * För insamling, processning och visualisering av mätdata.
* GitHub Actions:
    * För testautomatisering och kontinuerlig integration (CI).

---
