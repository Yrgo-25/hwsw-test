# Bilaga B - Övningsuppgifter

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
* `ci/build.sh` samt `.github/workflows/ci.yml` är skrivna för det här repots struktur (t.ex.
  `libs/test`, `libs/atmega/test`). Anpassa `ci/build.sh` så att det bygger och kör er egen
  testsvit i stället. I `.github/workflows/ci.yml` behöver ni åtminstone se över
  `submodules: true`, som bara behövs om ni själva har `yrgo::test` som submodul. Ni kommer
  tillbaka till resten av pipelinen, inklusive ESP-IDF-bygget, i **L13**.

---

## Tankeexperiment: `Logic` utan dependency injection
Eftersom **P02** redan krävde den här typen av design finns det sannolikt inget att refaktorera
i er egen kodbas. Resonera i stället kring motsatsen: anta att `system::logic::Logic` i stället
hade sett ut så här:

```cpp
class Logic
{
public:
    Logic() : myGpio{driver::gpio::Esp32s3{}} {}
private:
    driver::gpio::Esp32s3 myGpio;
};
```

* Vad hade behövt ändras i konstruktorn och medlemsvariablerna för att göra klassen testbar
  igen, utan att ändra *vad* klassen gör?
* Skulle det räcka att byta ut `driver::gpio::Esp32s3` mot `driver::gpio::Interface&`, eller
  skulle hela factory-mönstret (som er `system::logic::Logic` faktiskt använder) behövas för att
  hantera alla klassens drivers på ett enhetligt sätt? Jämför gärna med övningsbibliotekets
  `logic::Logic`, som löser samma problem utan factory, genom att ta emot tio enskilda
  interface-referenser i konstruktorn.

---

## Diskussion
* Varför räcker det med ett interface och en referens för att göra `Tmp36` testbar, medan
  `Logic` (som beror på flera olika drivers) i stället tar emot en hel factory?
* Vilken skillnad hade det gjort för testbarheten om `main` hade skapat drivers och skickat in
  dem en och en till `Logic`, i stället för att skicka in en factory?

---
