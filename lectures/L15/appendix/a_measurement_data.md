# Bilaga A - Processning och strukturerad lagring av mätdata

## Varför strukturerad lagring?
Rådata som bara skrivs ut i en terminal försvinner så fort fönstret stängs. Genom att spara
mätdatan strukturerat, t.ex. i en CSV-fil med tydliga kolumnrubriker, blir den både
återanvändbar och möjlig att bearbeta i efterhand, av er själva eller av någon annan.

```
timestamp_ms,adc_raw,temperature_c
1523,612,23.7
1541,614,23.8
1600,615,23.9
```

Notera att kolumnrubrikerna gör filen självförklarande, till skillnad från rådataformatet i
**L14**, där betydelsen bara fanns i huvudet på den som skrev koden.

---

## Enkel processning: glidande medelvärde
Rå mätdata innehåller ofta brus. Ett glidande medelvärde (*rolling average*) är ett enkelt sätt
att jämna ut den, genom att varje punkt ersätts med medelvärdet av sig själv och de `window`
närmast **föregående** mätningarna, alltså `window + 1` värden totalt. Fönstret är bakåtblickande
och tittar aldrig på framtida mätvärden, vilket gör att samma funktion kan användas live på data
som strömmar in. De första punkterna medelvärdesbildas över färre värden, eftersom det inte finns
tillräckligt många föregående mätningar än:

```python
def rolling_average(values, window):
    result = []
    for i in range(len(values)):
        first_index = max(0, i - window)
        window_values = values[first_index : i + 1]
        result.append(sum(window_values) / len(window_values))
    return result
```

Ett räkneexempel med `window = 2`, alltså det aktuella värdet plus de två föregående:

```text
Index:      0    1    2    3    4    5    6
Värden:    10   12   11   13   12   11   10
                 |_________|
                i-2        i       ->  (12 + 11 + 13) / 3 = 12.0
```

| `i` | `first_index = max(0, i - 2)` | `values[first_index : i + 1]` | Medelvärde |
|---|---|---|---|
| 0 | 0 | `[10]` | 10.00 |
| 1 | 0 | `[10, 12]` | 11.00 |
| 2 | 0 | `[10, 12, 11]` | 11.00 |
| 3 | 1 | `[12, 11, 13]` | 12.00 |
| 4 | 2 | `[11, 13, 12]` | 12.00 |
| 5 | 3 | `[13, 12, 11]` | 12.00 |
| 6 | 4 | `[12, 11, 10]` | 11.00 |

Notera de tre första raderna: eftersom det inte finns två föregående mätningar än
medelvärdesbildas de över färre värden. Det är ett medvetet val i koden ovan, via `max(0, ...)`.
Alternativet hade varit att inte producera något värde alls för de första punkterna, vilket ger
en kortare utdatalista än indatalistan, något som lätt ställer till det när ni sedan plottar
mätdatan mot sina tidsstämplar i **L16**.

---

## Datavaliditet
Vid inläsning av verklig mätdata bör ni ta höjd för att en rad kan vara skadad eller
ofullständig (se **L14**). Bestäm i förväg hur sådana rader ska hanteras: hoppas de över
tyst, loggas som en varning, eller får processningen avbrytas helt? Vilket som är rimligast
beror på hur kritisk datan är.

---
