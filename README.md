# Mjuk- och hårdvarutestning
Repo för kursen **Mjuk- och hårdvarutestning** (kurskod `Ee14H26`) med klassen Ee25, ht26–vt27.

Kursen genomförs parallellt med kursen **Maskininlärning**. De två kursernas praktiska prov är
gemensamma (se [Examination](#examination)), och den här kursens projekt bygger vidare på
kodbaser från både *Hårdvarunära programmering C/C++* (**P02**) och Maskininlärning (**P03**).

---

## Om kursen
Kursen behandlar testning och verifiering av mjukvara, på både enhets- och systemnivå, samt
automatisering av test, verifiering och mätdatainsamling för en DUT (*Device Under Test*). Fokus
ligger på:
* Metoder och verktyg för mjukvarutestning: enhetstester, komponenttester samt
  HW/SW-integrationstester.
* Verktyg som säkerställer programmets korrekthet, t.ex. statisk analys och sanitizers.
* Testbar design: interfaces, dependency injection samt stubbar/virtuell hårdvara.
* Testautomatisering och kontinuerlig integration (CI).
* Mätdatainsamling, processning och visualisering av mätdata för direkt analys och jämförelse
  mot kravställning.
* Utveckling av säker och robust programvara.

Under kursens gång implementeras:
* Enhetstester för de sex hårdvarunära drivers från **P02** (adc, factory, gpio, serial,
  tempsensor, timer) för `ESP32-S3`, både för deras befintliga stubbar och, via mockning av
  ESP-IDF, för minst en riktig `Esp32s3`-driver.
* Enhetstester för ML-algoritmer (**P03**): adaptiv linjär regression samt neurala
  nätverkslager.
* Komponenttester för `system::logic::Logic`, som kombinerar flera stubbade drivers respektive
  ML-lager.
* En CI-pipeline, byggd kring det medföljande testramverket `yrgo::test`, som möjliggör
  kontinuerlig testning genom hela produktens livscykel.
* Insamling, processning och visualisering av mätdata från fysisk hårdvara.

Efter genomförd kurs ska studenten kunna:
* Beskriva olika metoder och verktyg för testning av mjukvara.
* Redogöra för verktyg som säkerställer programmets korrekthet.
* Specificera och automatisera relevanta testfall för en DUT, utifrån dess kravställning.
* Konstruera tester på enhets- och systemnivå.
* Utveckla programkod för mätdatainsamling, processning och visualisering av mätresultat.
* Utveckla säker och robust programvara samt ett testramverk för kontinuerlig testning.

---

## Struktur

```text
ci/          Skript för kodformattering och kontroll av testramverket
info/        Kursinformation, schema och examination
lectures/    Föreläsningar och övningsuppgifter
libs/        Delade C++-bibliotek: testramverket yrgo::test (git-submodul) samt ett
             ATmega328p-övningsbibliotek för L02–L04 samt L08
projects/    Projektbeskrivning, krav och inlämningsinstruktioner
```

---

## Examination
Kursen examineras genom:
* Ett projekt, **P04**, se [projects/P04](./projects/P04/README.md).
* Två praktiska prov, gemensamma med kursen [Maskininlärning](https://github.com/Yrgo-25/Machine-Learning), se [Praktiska prov](./info/README.md#praktiska-prov) i kursinfon.

Fullständig information om schema och poängfördelning finns i [info](./info/README.md).

---

## Kodformattering
`ci/format.sh` formaterar C/C++-kod med `clang-format` samt Python-kod med `black`:

```bash
ci/format.sh          # Formatera alla filer.
ci/format.sh --check  # Kontrollera formattering utan att ändra filer.
```

Innan skriptet körs behöver `clang-format` samt `black` finnas installerade och tillgängliga i
`PATH`.

`clang-format` installeras via `apt` i WSL:

```bash
sudo apt -y update
sudo apt -y install clang-format
```

Kursens Python-beroenden (`black`, samt `matplotlib` och `pyserial` för mätdatamomenten i
**L14–L16**) installeras via `requirements.txt`:

```bash
pip install -r requirements.txt
```

---

## Testramverket `yrgo::test`
Kursens enhets- och komponenttester skrivs med `yrgo::test`, ett lättviktigt C++17-ramverk som
används genomgående i kursen (se [libs/test](./libs/test/README.md)). Ramverket byggs och
verifieras automatiskt via `ci/build.sh`, som även bygger och kör
[ATmega328p-övningsbibliotekets](./libs/atmega/README.md) testsvit:

```bash
ci/build.sh   # Bygger libyrgo_test.a samt bygger och kör dess bifogade exempel och den
              # aktiverade delen av ATmega328p-övningsbibliotekets testsvit. Tester som
              # fortfarande ligger bakom ett #ifdef LECTUREn kompileras alltså inte in.
```

---
