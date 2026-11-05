# Bilaga A - Manuella HW/SW-integrationstester

## Vad kan automatiseras, vad kräver en människa?
Integrationstesterna i **L18** kontrollerar systemet via den seriella porten, och kan därför
köras av ett skript. Men allt ett system gör syns inte i det det skriver ut:
* Ett test som skickar kommandon över seriell kommunikation och verifierar svaret går ofta att
  automatisera med `pytest` (se **L18**).
* Ett test som kräver att en människa fysiskt trycker på en knapp, mäter en spänning med ett
  instrument, jämför mot en referenstermometer, eller bedömer om en lysdiod faktiskt lyser med
  rätt styrka, kräver manuell verifiering.

Gränsen går alltså vid vad systemet kan *observera om sig självt*. Firmwaren kan rapportera att
den har satt en pin hög, men inte att lysdioden på den pinnen faktiskt lyser; det kräver ett
öga eller ett instrument utanför systemet.

Det är fullt rimligt, och förväntat, att en del av er integrationstestning i **P04** är manuell.
Det viktiga är att den då **dokumenteras** på ett spårbart sätt.

---

## Mall för ett manuellt testfall
Ett manuellt testfall bör dokumenteras minst lika tydligt som ett automatiserat, annars kan
det inte upprepas på samma sätt av någon annan (eller av er själva om ett halvår):
* **Syfte.** Vad ska testfallet verifiera, och vilket krav hör det till?
* **Förutsättningar.** Vilken hårdvara, firmware-version och utrustning krävs, och i vilket
  utgångsläge ska systemet vara?
* **Genomförande.** Exakt vilka steg utfördes, i vilken ordning?
* **Förväntat resultat.** Vad *skulle* hända, formulerat innan testet körs?
* **Faktiskt resultat.** Vad hände i praktiken, med uppmätta värden?
* **Bedömning.** Godkänt eller underkänt, och vem som utförde testet när.

Det är de sex fälten ni ska använda i övningsuppgiften i [bilaga B](./b_exercises.md).

Notera likheten med ett automatiserat testfall: *förutsättningar* motsvarar fixturen som ger
ett känt utgångsläge, *genomförande* motsvarar *Act*, och *förväntat resultat* motsvarar
`assert`. Skillnaden är bara vem som utför stegen.

---

## Spårbarhet: från krav till testfall
När testsviten består av enhets-, komponent-, automatiserade och manuella integrationstester är
det lätt att tappa bort vilket krav som verifieras var. En enkel **spårbarhetstabell** i ert
repo håller ordning på det:

| Krav | Verifieras av | Typ |
|---|---|---|
| `"on"` tänder LED:en | `logic_test.cpp`: `on_command_sets_led_high` | Komponenttest |
| `"on"` tänder LED:en | `tests/hil/test_commands.py`: `test_on_turns_led_on` | Automatiserat integrationstest |
| LED:en lyser synligt efter `"on"` | `docs/manual_tests.md`: MT-01 | Manuellt integrationstest |
| Temperaturen avviker högst ±1 °C | `docs/manual_tests.md`: MT-02 | Manuellt integrationstest |

Tabellen gör två saker synliga på en gång: krav som saknar test helt, och krav som bara
verifieras manuellt och därför är värda att försöka automatisera.

---
