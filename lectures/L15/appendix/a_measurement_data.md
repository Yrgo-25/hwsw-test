# Bilaga A - Mätdatainsamling, lagring och visualisering

![](./images/data_collection.png)

## DUT: Device Under Test
En **DUT** är den fysiska enhet som testas eller mäts, i vårt fall er `ESP32-S3`. Till skillnad
från enhets- och komponenttester, som körs på värddatorn utan hårdvara, handlar
mätdatainsamling om att faktiskt hämta ut data från den riktiga, körande produkten.

Kedjan består av tre steg, som vart och ett motsvarar en del av VG-kravet i **P04**:
1. **Insamling:** `ESP32-S3` skickar mätvärden över seriell kommunikation, och ett Python-skript
   läser dem på värddatorn.
2. **Lagring:** skriptet sparar mätvärdena strukturerat, t.ex. som CSV.
3. **Visualisering:** mätdatan ritas upp och jämförs mot systemets kravställning.

---

## Insamling: ett enkelt, väldefinierat protokoll
För att värddatorn ska kunna tolka data som skickas över seriell kommunikation krävs ett
väldefinierat format, känt av båda sidor i förväg. Ett enkelt och vanligt val är
kommaseparerade rader, en mätning per rad, avslutad med radbrytning:

```
1523,612,23.7
1541,614,23.8
1600,615,23.9
```

Formatet i sig känner inte till vad kolumnerna betyder; det är en överenskommelse mellan den som
skriver `ESP32`-firmwaren och den som skriver Python-skriptet. En kort läsning av sådana rader i
Python med `pyserial` kan se ut så här:

```python
import serial

with serial.Serial("/dev/ttyUSB0", baudrate=115200, timeout=1) as port:
    while True:
        line = port.readline().decode().strip()
        if line:
            print(line)
```

**Noteringar:**
* `readline()` returnerar de mottagna tecknen som en sekvens av bytes.
* `decode()` omvandlar byten till en vanlig Python-sträng (`str`) genom att tolka dem som UTF-8.
* `strip()` tar bort den avslutande radbrytningen (`\n`) som markerar slutet på varje mätning.

Bestäm i förväg hur skriptet ska hantera en rad som är trasig eller ofullständig, t.ex. på grund
av brus på linjen: hoppas den över tyst, loggas den som en varning, eller avbryts insamlingen?

---

## Lagring: CSV med kolumnrubriker
Rådata som bara skrivs ut i en terminal försvinner så fort fönstret stängs. Spara den i stället
som en CSV-fil med tydliga kolumnrubriker, inklusive enhet, så blir filen självförklarande och
går att bearbeta i efterhand:

```
timestamp_ms,adc_raw,temperature_c
1523,612,23.7
1541,614,23.8
1600,615,23.9
```

Är mätdatan brusig kan ett glidande medelvärde jämna ut den, men var medvetna om att samma
utjämning även kan dölja ett verkligt problem. Spara därför alltid rådatan, och processa en kopia.

---

## Visualisering och jämförelse mot kravställning
En graf säger ofta mer vid en snabb anblick än en tabell med hundratals mätvärden. Ett
linjediagram passar tidsseriedata, t.ex. temperatur över tid. Rita in kravet i samma graf som
mätdatan, så syns det direkt om systemet höll sig inom vad som krävdes:

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

---
