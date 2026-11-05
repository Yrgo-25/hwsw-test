# Bilaga A - Automatiserade HW/SW-integrationstester med Python

## Samma scenario, en ny testnivå
I **L11** skrev ni komponenttester för `system::logic::Logic`, t.ex. att kommandot `"on"` sätter
`driver::gpio::Stub` till hög nivå. Det testet säger mycket om `Logic`, men ingenting om huruvida
den riktiga UART:en tar emot kommandot, om rätt pin faktiskt är konfigurerad, eller om firmwaren
överhuvudtaget startar på kortet.

Ett **HW/SW-integrationstest** kör samma scenario mot det färdiga systemet: den riktiga
firmwaren, på ett riktigt `ESP32-S3`-kort, via den riktiga seriella porten. Testet ser systemet
utifrån, precis som en användare: det skickar ett kommando och kontrollerar svaret.

| | Komponenttest (**L11**) | HW/SW-integrationstest |
|---|---|---|
| Körs på | Värddatorn | `ESP32-S3` |
| Drivers | Stubbar | Riktiga `Esp32s3`-drivers |
| Skrivs i | C++, `yrgo::test` | Python, `pytest` |
| Ser | Stubbarnas interna tillstånd | Endast det systemet skickar ut |
| Körtid | Millisekunder | Sekunder |
| Deterministiskt | Ja | Nästan: brus, tidsavvikelser och kortets tillstånd påverkar |

Den sista raden är skälet till att integrationstesterna kompletterar, men aldrig ersätter, de
lägre testnivåerna: ett fel som går att hitta med ett komponenttest ska hittas där.

---

## `pytest`
Ni har redan skrivit Python-skript som läser från porten (**L16**) och gör en bedömning med en
returkod (**L17**). Det fungerar för ett test, men så snart ni har tio blir det samma problem som
med testfall i C++ utan ramverk: varje skript måste själv hålla reda på vad som gick fel, och ett
misslyckat test stoppar resten.

`pytest` är det de facto vanligaste testramverket för Python, och fyller samma roll som
`yrgo::test`:
* Varje funktion vars namn börjar med `test_`, i en fil vars namn börjar med `test_`, är ett
  testfall.
* En vanlig `assert` är testets kontroll. Misslyckas den rapporterar `pytest` vilka värden som
  jämfördes.
* Alla testfall körs, även om ett tidigare har misslyckats, och en sammanställning skrivs ut
  sist.
* Returkoden är `0` om allt gick igenom, och skild från `0` annars.

`pytest` installeras via `requirements.txt`, precis som `pyserial`.

---

## Struktur
Lägg integrationstesterna i en egen katalog, skild från era C++-tester:

```text
tests/
└── hil/
    ├── conftest.py      Gemensam uppsättning: portargument och fixtures.
    ├── dut.py           Hjälpfunktioner för att kommunicera med DUT:en.
    └── test_commands.py Testfallen.
```

### Kommunikation med DUT:en
Samla det som handlar om protokollet på ett ställe, så att testfallen bara beskriver *vad* som
testas:

```python
# dut.py
import serial


def send_command(port: serial.Serial, command: str) -> str:
    """Send a command to the DUT and return the first line of its response."""
    port.reset_input_buffer()
    port.write(f"{command}\n".encode())
    return port.readline().decode().strip()
```

Anpassa funktionen efter er firmware: vilket radslut den förväntar sig (`\n` eller `\r\n`), om
den ekar tillbaka kommandot innan svaret, och om svaret är en eller flera rader.

### Fixtures: porten och ett känt utgångsläge
En **fixture** är `pytest`s motsvarighet till den uppsättning ni gjort i början av varje
C++-testfall (*Arrange*): ett objekt som testfallet tar emot som parameter, och som `pytest`
skapar åt det. Två fixtures räcker långt:

```python
# conftest.py
import pytest
import serial

from dut import send_command


def pytest_addoption(parser):
    parser.addoption("--port", default=None, help="Serial port of the DUT, e.g. /dev/ttyUSB0.")


@pytest.fixture(scope="session")
def port(request):
    """Open the serial port once for the whole test session."""
    port_name = request.config.getoption("--port")
    if port_name is None:
        pytest.skip("No DUT connected, run with --port to enable the HIL tests.")
    with serial.Serial(port_name, baudrate=115200, timeout=1) as connection:
        yield connection


@pytest.fixture
def dut(port):
    """Put the DUT in a known state before each test."""
    send_command(port, "blink off")
    send_command(port, "off")
    return port
```

**Noteringar:**
* `scope="session"` gör att porten öppnas en gång för hela körningen. På många kort startar
  `ESP32-S3` om när porten öppnas, och det vill ni inte ska ske inför varje testfall.
* `dut` körs i stället före *varje* testfall och återställer systemet till ett känt
  utgångsläge. Det är samma princip som att en stubbs utgångsläge måste vara känt (se **L09**):
  utan det beror ett testresultat på vilket test som råkade köras innan.
* Saknas `--port` hoppas testerna över i stället för att misslyckas. Då kan sviten ligga i repot,
  och köras i en pipeline, även på en maskin utan kort inkopplat.

---

## Testfallen
Med hjälpfunktionen och fixturerna på plats blir testfallen korta, och följer samma
Arrange-Act-Assert-struktur som i C++:

```python
# test_commands.py
from dut import send_command


def test_on_turns_led_on(dut):
    """Requirement: the command "on" turns the LED on."""
    send_command(dut, "on")
    assert "led: on" in send_command(dut, "status")


def test_temperature_is_plausible(dut):
    """Requirement: "temp" reports a temperature within the sensor's indoor range."""
    response = send_command(dut, "temp")
    temperature = float(response.split()[-1])
    assert 15.0 <= temperature <= 35.0


def test_unknown_command_is_rejected(dut):
    """Requirement: an unknown command is reported as an error, not silently ignored."""
    assert "error" in send_command(dut, "foo").lower()
```

De förväntade strängarna (`"led: on"`, `"error"`) är exempel; använd det er firmware faktiskt
skriver ut. Notera två skillnader mot era komponenttester:
* **Testet kan bara se det systemet skickar ut.** Komponenttestet kunde läsa av
  `driver::gpio::Stub` direkt; här måste LED:ens tillstånd gå att fråga efter, t.ex. via
  `"status"`. Saknar er firmware ett sådant sätt kan testet inte verifiera LED:en automatiskt,
  och det är ett exempel på när ett manuellt test behövs (se **L19**).
* **Toleranser i stället för exakta värden.** Temperaturen i rummet är okänd, så testet
  kontrollerar ett rimligt intervall. Det fångar en trasig sensor eller en felräknad formel, men
  inte ett litet systematiskt fel; det hör hemma i mätningen mot referenstermometer (se **L17**).

### Mätningar som testfall
Den automatiska bedömningen från **L17** passar lika bra som testfall. Ett test som samlar in
blinkperioderna under tio sekunder och kontrollerar att den största avvikelsen håller sig inom
kravet är ett integrationstest, och kan köras tillsammans med resten av sviten.

---

## Körning och testrapport
Sviten körs från repots rot:

```bash
pytest tests/hil --port /dev/ttyUSB0
```

Med flaggan `--junitxml=report.xml` skriver `pytest` dessutom en testrapport i JUnit-formatet,
som de flesta CI-system, inklusive GitHub Actions via färdiga actions, kan visa som en tabell
över godkända och underkända testfall. Spara rapporten bredvid mätdatan: den är ett spårbart
bevis på vilka tester som kördes mot vilken firmware, och med vilket resultat.

---

## Instabila tester
Ett integrationstest som ibland blir rött utan att något har ändrats, ett *flaky* test, är
värre än inget test alls: efter några falsklarm slutar gruppen att ta röda resultat på allvar.
De vanligaste orsakerna är:
* **För korta tidsgränser.** Ett svar som oftast kommer inom 100 ms kommer ibland efter 150 ms.
* **Okänt utgångsläge**, t.ex. kvarvarande tecken i mottagningsbufferten från ett tidigare
  testfall. Det är därför `send_command()` tömmer bufferten innan kommandot skickas.
* **För snäva toleranser** jämfört med den spridning ni faktiskt mätte upp i **L17**.

Lös orsaken i stället för att köra om testet tills det blir grönt. Ett test som behöver köras tre
gånger för att gå igenom döljer ett verkligt problem, antingen i testet eller i systemet.

---

## Hela vägen: self-hosted runner
Pipelinen från **L05** kör på en GitHub-hostad runner (`runs-on: ubuntu-latest`), en
virtuell maskin utan något kort inkopplat. Den kan bygga firmwaren och spara den som artefakt, men
inte flasha den. Demot i **L05** körde på en self-hosted runner, men skillnaden var då bara var
maskinen stod, inte vad den kunde göra.

Gränsen går dock inte vid tekniken, utan vid var runnern står. GitHub Actions kan lika gärna köra
jobb på en maskin ni själva äger, en **self-hosted runner**. Står den maskinen med en `ESP32-S3`
inkopplad via USB kan ett jobb hämta firmware-artefakten, flasha kortet med `esptool.py` och köra
`pytest`-sviten ovan mot den seriella porten, helt automatiskt vid varje push. Det kallas
*hardware-in-the-loop* (HIL) och är så här hårdvarutestning automatiseras i industrin.

Att registrera runnern tar bokstavligen någon minut, och ni såg det göras i **L05**: maskinen
registreras under *Settings → Actions → Runners* och får en egen etikett, som jobbet sedan begär
via `runs-on:`. Det som gör tekniken krävande är allt därefter:
* **Maskinen måste vara igång.** Är den avstängd när någon pushar blir jobbet stående i kö, precis
  som ni såg demonstrerat i **L05**. En pipeline som fastnar eller blir röd av skäl som inte har med
  koden att göra slutar snabbt att tas på allvar.
* **Hårdvaran måste vara i ett känt utgångsläge.** Ett kort som hängt sig sedan förra körningen
  gör nästa testresultat värdelöst.
* **Säkerhet.** En self-hosted runner kör kod från repot på er egen maskin. På ett publikt repo
  kan vem som helst öppna en pull request och därmed köra godtycklig kod hos er, vilket är
  anledningen till att GitHub avråder från self-hosted runners på publika repon.

Upplägget ingår inte i kraven för **P04**, eftersom det förutsätter att varje grupp har ett kort
permanent inkopplat till en maskin som alltid är igång. Sviten ni skriver är däremot exakt den som
en sådan runner skulle köra: det enda som skiljer är vem som skriver `pytest` i terminalen. Poängen
är att ni ska veta att gränsen mellan "automatiserbart" och "kräver en människa" går att flytta,
och vad det kostar att flytta den.

---
