# Kursinfo

## Lärare
Erik Pihl ([erik.axel.pihl@gmail.com](mailto:erik.axel.pihl@gmail.com))

**L16–L19** hålls av en gästlärare, **L20** hålls av båda. **P04** bedöms av kursansvarig lärare
tillsammans med gästläraren.

---

## Kursplan - Mjuk- och hårdvarutestning

| Vecka | Datum | Föreläsning | Ämne |
|-------|-------|-------------|------|
| 34 | 19/8 | L01 | Kursintroduktion, teststrategi och testpyramiden |
| 34 | 20/8 | L02 | Enhetstester (del I): grunder samt GPIO-drivern, introduktion till **P04** |
| 35 | 26/8 | L03 | Enhetstester (del II): den seriella drivern, arbete med **P04** |
| 36 | 2/9 | L04 | Enhetstester (del III): timer-drivern, arbete med **P04** |
| 37 | 9/9 | L05 | CI-uppstart: automatiserad testkörning samt `clang-format`, arbete med **P04** |
| 38 | 16/9 | L06 | Testbar design: dependency injection, stubbar och virtuell hårdvara, arbete med **P04** |
| 38 | 17/9 | L07 | Referensvärden och toleranser: `Tmp36`s temperaturformel samt ML-algoritmerna, arbete med **P04** |
| 39 | 23/9 | L08 | Komponenttester (del I): vad ett komponenttest är, samt `driver::tempsensor::Stub`, arbete med **P04** |
| 40 | 30/9 | L09 | Komponenttester (del II): `driver::gpio::Stub` samt `driver::timer::Stub`, arbete med **P04** |
| 41 | 7/10 | L10 | Arbete med praktiskt prov 1 (gemensamt med Maskininlärning) |
| 2 | - | L11 | Komponenttester (del III): systemlogiken och stubbar med styrbar utdata, arbete med **P04** |
| 2 | - | L12 | Komponenttester (del IV): gränsvärdesanalys, robusthet och sanitizers, arbete med **P04** |
| 3 | - | L13 | Kodkvalitet i pipelinen: statisk analys, branch protection och jobbordning, arbete med **P04** |
| 3 | - | L14 | Projektarbete (del I): **P04** |
| 4 | - | L15 | Projektarbete (del II): **P04** |
| 4 | - | L16 | Mätdatainsamling (del I): insamling och lagring, arbete med **P04** (gästlärare) |
| 5 | - | L17 | Mätdatainsamling (del II): processning och visualisering, arbete med **P04** (gästlärare) |
| 5 | - | L18 | Automatiserade HW/SW-integrationstester med Python, arbete med **P04** (gästlärare) |
| 6 | - | L19 | Manuella integrationstester och projektarbete (del III): **P04** (gästlärare) |
| 6 | - | L20 | Redovisning av **P04** och kursavslut (gästlärare + huvudlärare) |

**OBS!** Kursen är vilande från vecka 42 fram till årsskiftet; L11 återupptas i januari. Veckor
för L11–L20 är preliminära och kan komma att justeras. Praktiskt prov 2 examineras under
Maskininlärning-kursens **L20**, och har ingen egen lektion i den här kursen (se
[Praktiska prov](#praktiska-prov)).

---

## Examination och betygsnivåer

### Upplägg
* Ett projekt (**P04**).
* Två praktiska prov, gemensamma med kursen Maskininlärning, genomförda som inlämningsuppgifter
  med muntlig redovisning.

### Praktiska prov
Båda proven är gemensamma med kursen [Maskininlärning](https://github.com/Yrgo-25/machine-learning).
Provet består av en gemensam uppgiftsuppsättning per tillfälle: en delmängd av uppgifterna
examinerar maskininlärning, en annan delmängd examinerar testning. Betyg sätts separat per kurs,
utifrån den delmängd av uppgifterna som hör till respektive kurs.

Proven genomförs på egen hand fram till angiven deadline, och redovisas därefter muntligt: ni visar
er egen kod och förklarar den. AI-verktyg får användas under arbetet, men koden ska skrivas av er
själva, och ni ska kunna redogöra för varje test och varje rättning ni lämnar in.

Varje uppgift är märkt med vilken kurs den examinerar samt med **G** eller **VG**. Betyget avgörs
av vilka uppgifter ni genomför: G-uppgifterna ger ett **G**, och samtliga uppgifter ger ett **VG**.
En uppgift räknas som genomförd när den är klar och fungerar, dvs. testfallen går igenom och
rättningarna är på plats.

Denna kurss del av respektive prov täcker:
* **Prov 1:** Enhets- och komponenttester av ett neuralt nätverks dense-lager, skrivna med
  `yrgo::test`, samt felsökning av en färdig implementation med hjälp av dessa tester.
* **Prov 2:** Enhets- och komponenttester av konvolutionella lager (Conv, MaxPool, Flatten) samt
  ett helt CNN uppbyggt via stubbar.

Prov 1 är utdelat; länken till provet finns i Maskininlärning-kursens Classroom. **L10** är
avsatt för att arbeta med provet och slutföra det.

Prov 2 ([hemtentamen 2](https://github.com/Yrgo-25/machine-learning/blob/main/exams/exam2/home_exam2.md))
delas ut i början av januari, i samband med Maskininlärning-kursens **L11**, och examineras under
Maskininlärning-kursens **L20**. Ni har alltså flera veckor på er, och provet genomförs i övrigt på
samma sätt som prov 1: som en inlämningsuppgift på egen hand, med muntlig redovisning. Den här
kursen har ingen egen lektion för provet, men provet ska fortfarande genomföras, och
testuppgifterna betygssätts som en del av den här kursen, precis som för prov 1.

Övningstentamen inför prov 1, inklusive den kod ni ska utgå från, delas mellan kurserna och finns i
Maskininlärning-kursens repo:
* [Övningstentamen 1](https://github.com/Yrgo-25/machine-learning/blob/main/exams/exam1/practice_exam1.md): uppgift 1, 2, 4, 6 och 7 examinerar
  testning. Gås igenom i Maskininlärning-kursens **L10**.
* [Hemtentamen 2](https://github.com/Yrgo-25/machine-learning/blob/main/exams/exam2/home_exam2.md) utgör prov 2: uppgift 1, 2 och 4 examinerar
  testning.

### Poängfördelning
* **P04** ger upp till 4 poäng per student (**G** = 2p, **VG** = 4p), satta individuellt enligt
  kriterierna i [projektbeskrivningen](../projects/P04/README.md#bedömning).
* **Praktiskt prov 1** ger upp till 2 poäng (**G** = 1p, **VG** = 2p), baserat på
  testuppgifterna i provet.
* **Praktiskt prov 2** ger upp till 2 poäng (**G** = 1p, **VG** = 2p), baserat på
  testuppgifterna i provet.

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
* Multimeter samt referenstermometer: för de manuella integrationstesterna i **L19** (VG), där
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
* AddressSanitizer/UndefinedBehaviorSanitizer (ASan/UBSan):
    * Körtidsverktyg för att hitta minnesfel och odefinierat beteende.
* Python 3 med `matplotlib`, `pyserial` samt `pytest`:
    * För insamling, processning och visualisering av mätdata, samt automatiserade
      HW/SW-integrationstester (**L16–L18**).
* GitHub Actions:
    * För testautomatisering och kontinuerlig integration (CI).

---
