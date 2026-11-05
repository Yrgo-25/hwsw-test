# Bilaga A - Processning och visualisering av mätdata

## Från mätdata till ett svar på en fråga
En CSV-fil med tusen rader är inget resultat i sig. Den blir ett resultat först när den besvarar
en fråga, och i testsammanhang är frågan nästan alltid densamma: **uppfyller systemet sin
kravställning?** Kedjan i den här bilagan går därför från ett krav, via processning, till en graf
och en bedömning.

---

## Steg 1: ett mätbart krav
Ett krav som "temperaturen ska vara korrekt" eller "LED:en ska blinka jämnt" går inte att jämföra
mätdata mot. Formulera om det så att det anger ett **nominellt värde** och en **tolerans**:
* *Den predikterade temperaturen ska avvika högst ±1 °C från referenstermometerns värde.*
* *Blinkperioden ska vara 500 ms ± 5 ms, mätt mellan två på varandra följande togglingar.*

Finns inget krav angivet i er kravställning från **P02**/**P03** får ni formulera ett själva, och
motivera det. Ett krav ni själva har satt är fullt godtagbart, så länge det är mätbart och
dokumenterat innan ni tittar på mätdatan; ett krav som anpassas efter resultatet är inget krav.

---

## Steg 2: processning
Läs in CSV-filen från **L16** och räkna fram de storheter kravet handlar om. Python-modulen `csv`
räcker långt, tillsammans med `statistics` ur standardbiblioteket:

```python
import csv
import statistics

timestamps = []
temperatures = []

with open("measurements.csv", newline="") as file:
    for row in csv.DictReader(file):
        timestamps.append(float(row["timestamp_ms"]))
        temperatures.append(float(row["temperature_c"]))

print(f"Samples: {len(temperatures)}")
print(f"Mean:    {statistics.mean(temperatures):.2f} °C")
print(f"Stdev:   {statistics.stdev(temperatures):.2f} °C")
print(f"Min/max: {min(temperatures):.2f} / {max(temperatures):.2f} °C")
```

Vilka storheter som är intressanta beror på kravet:
* **Medelvärde** säger om systemet i genomsnitt hamnar rätt, dvs. om det har ett systematiskt fel.
* **Standardavvikelse** säger hur mycket mätvärdena sprider sig, dvs. hur brusigt systemet är.
* **Min och max**, eller den största avvikelsen från det nominella värdet, är oftast det kravet
  egentligen handlar om: ett system som i genomsnitt blinkar med rätt period men ibland missar med
  50 ms uppfyller inte ett krav på ±5 ms.
* **Differenser** mellan på varandra följande rader, t.ex. tiden mellan två togglingar, behövs när
  kravet gäller en period eller ett intervall snarare än ett enskilt värde.

Precis som i **L16**: processa en kopia, och låt rådatan ligga kvar orörd.

---

## Steg 3: visualisering
En graf säger ofta mer vid en snabb anblick än en tabell med hundratals mätvärden. Välj
diagramtyp efter frågan:
* **Linjediagram** passar tidsseriedata, t.ex. temperatur över tid. Det visar trender och
  enstaka avvikande mätvärden.
* **Histogram** passar fördelningen av ett värde, t.ex. hur blinkperioderna fördelar sig kring
  500 ms. Det visar spridning och om avvikelserna är jämnt fördelade eller samlade åt ett håll.

Rita in kravet i samma graf som mätdatan, så syns det direkt om systemet höll sig inom vad som
krävdes:

```python
import matplotlib.pyplot as plt

# Plot the measured temperatures.
plt.plot(timestamps, temperatures, label="Predicted temperature")

# Draw the upper and lower requirement limits.
plt.axhline(y=expected_temp + 1, color="r", linestyle="--", label="Requirement limit (±1 °C)")
plt.axhline(y=expected_temp - 1, color="r", linestyle="--")

# Add a title and axis labels.
plt.title("Predicted temperature over time")
plt.xlabel("Time (ms)")
plt.ylabel("Temperature (°C)")

# Show the legend and save the figure.
plt.legend()
plt.savefig("temperature_plot.png")
```

Några saker som skiljer en användbar graf från en vacker:
* **Axlar med storhet och enhet.** En graf utan enheter går inte att tolka för någon annan.
* **Kravet inritat**, med toleransen tydligt markerad.
* **Spara grafen till fil** med `savefig()`, i stället för att bara visa den med `show()`. Då kan
  den checkas in bredvid mätdatan och visas vid redovisningen, och skriptet kan köras utan
  bildskärm, t.ex. i CI.

---

## Steg 4: en automatisk bedömning
Grafen gör det lätt för en människa att se om kravet uppfylls. Men samma bedömning kan också
göras av skriptet självt, och då kan den upprepas exakt likadant vid varje mätning:

```python
import sys

max_deviation = max(abs(temp - expected_temp) for temp in temperatures)
passed = max_deviation <= 1.0

print(f"Max deviation: {max_deviation:.2f} °C -> {'PASS' if passed else 'FAIL'}")
sys.exit(0 if passed else 1)
```

Notera `sys.exit()`: ett skript som avslutas med returkod `0` vid godkänt och `1` vid underkänt
beter sig precis som er testsvit i C++, och kan därmed köras av en pipeline som avgör om något
blev rött. Det är bryggan mellan mätdatainsamling och automatiserade integrationstester, som är
temat i **L18**.

---
