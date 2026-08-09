# Bilaga A - Visualisering av mätdata

## Varför visualisera?
En tabell med hundratals mätvärden säger sällan lika mycket vid en snabb anblick som en graf
gör. Visualisering gör det lätt att upptäcka trender, avvikelser och om systemet uppfyller sin kravställning, utan att läsa varje enskild rad.

---

## Rätt diagramtyp för rätt data
* **Linjediagram** passar tidsseriedata, t.ex. temperatur över tid, där ordningen mellan
  punkterna är meningsfull.
* **Spridningsdiagram** (*scatter plot*) passar när ni vill undersöka sambandet mellan två
  variabler, t.ex. ADC-råvärde mot faktisk temperatur.

Ett enkelt exempel med `matplotlib`, som även visar hur ett kravtröskelvärde kan markeras direkt
i grafen:

```python
import matplotlib.pyplot as plt

# Plot the measured temperatures.
plt.plot(timestamps, temperatures, label="Predicted temperature")

# Draw the upper and lower requirement limits.
plt.axhline(
    y=expected_temp + 1,
    color="r",
    linestyle="--",
    label="Requirement limit (±1 °C)")
plt.axhline(
    y=expected_temp - 1,
    color="r",
    linestyle="--")

# Add a title and axis labels.
plt.title("Predicted temperature over time")
plt.xlabel("Time (ms)")
plt.ylabel("Temperature (°C)")

# Show the legend and save the figure.
plt.legend()
plt.savefig("temperature_plot.png")
```

## Jämförelse mot kravställning
En visualisering blir betydligt mer användbar om kraven ritas in i samma graf som mätdatan,
som kravgränserna i exemplet ovan. Då syns det direkt, utan vidare tolkning, om systemet höll
sig inom vad som krävdes eller inte.

---
