# Bilaga B - Övningsuppgifter

Uppgifterna nedan görs i grupp och i **ert eget repo**, och utgör den andra delen av G-kravet
[Mätdatainsamling och automatiserade integrationstester](../../../projects/P04/README.md#mätdatainsamling-och-automatiserade-integrationstester-g)
i **P04**. De gås inte igenom i helklass, eftersom varje grupps firmware och protokoll ser olika
ut; läraren går runt och hjälper till under tiden.

---

## Uppsättning
* Lägg till `pytest` i ert repos `requirements.txt` och installera det.
* Skapa katalogen `tests/hil/` med `conftest.py` och `dut.py` enligt
  [bilaga A](./a_automated_integration.md), och anpassa `send_command()` efter er firmware:
  radslut, eventuellt eko och svarets format.
* Kör `pytest tests/hil` **utan** `--port` och kontrollera att testerna hoppas över i stället för
  att misslyckas.

---

## Testfall (G)
* Skriv minst ett automatiserat integrationstest som skickar ett seriellt kommando till kortet och
  verifierar svaret, t.ex. att `"temp"` returnerar en rimlig temperatur.
* Ange i testets docstring vilket krav det verifierar.
* Kör sviten mot kortet med `--port`, och kontrollera att den går igenom.
* Kontrollera att testet faktiskt kan bli rött: ändra det förväntade värdet tillfälligt, eller
  flasha en firmware med ett medvetet fel, och kör sviten igen.

---

## Testfall (VG)
För den som siktar på VG-momentet *Ett testramverk för kontinuerlig testning*:
* Täck samtliga seriella kommandon i er firmware, inklusive ogiltiga kommandon och ogiltiga
  argument som `"period 0"`.
* Gör den automatiska bedömningen från **L17** till ett testfall i sviten, så att en mätning mot
  kravställningen körs tillsammans med övriga integrationstester.
* Generera en testrapport med `--junitxml` och checka in den tillsammans med vilken firmware
  (commit) den gäller.

---

## Diskussion
* Vilket av era komponenttester från **L11** motsvarar ert integrationstest? Vad kan
  integrationstestet hitta som komponenttestet inte kan, och tvärtom?
* Fanns det något i ert system ni ville testa, men inte kunde observera via den seriella porten?
  Hur skulle det i så fall verifieras (se **L19**)?

---
