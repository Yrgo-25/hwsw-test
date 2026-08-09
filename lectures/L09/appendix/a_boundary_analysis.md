# Bilaga A - Gränsvärdesanalys

## Varför gränsfall hittar fler buggar
De flesta buggar döljer sig inte vid typisk, "normal" indata, de döljer sig vid gränserna:
tomma listor, maxvärden, samtidiga händelser. **Gränsvärdesanalys** (*boundary value analysis*)
är en systematisk metod för att hitta dessa fall: för varje gräns i systemets beteende, testa
värdet precis vid gränsen, precis under den, samt precis över den.

Anta att ett giltigt antal element är `0..10`. Gränsvärdesanalys ger då sex testfall, tre vid
varje gräns. Notera att det är *värdet* som varieras, inte bara vilken gräns man tittar på:

```text
   ogiltigt |              giltigt område              | ogiltigt
 -----+-----+-----+-----+ ... +-----+-----+-----+------+----->
      -1     0     1            9    10    11
       ^     ^     ^            ^     ^     ^
       |     |     |            |     |     |
     under  vid  över         under  vid  över
       \_____ _____/            \_____ _____/
             v                        v
       undre gränsen            övre gränsen
```

Ett besläktat begrepp är **ekvivalenspartitionering**: att dela upp möjlig indata i grupper där
alla värden inom en grupp rimligen borde ge samma typ av resultat, och sedan bara testa ett
representativt värde per grupp i stället för varje enskilt möjligt värde. Samma exempel ger då
bara tre testfall:

| Partition | Värden | Typ | Representativt testvärde |
|---|---|---|---|
| Ogiltiga (låga) | …, −2, −1 | ogiltig | −1 |
| Giltiga | 0 … 10 | giltig | 5 |
| Ogiltiga (höga) | 11, 12, … | ogiltig | 11 |

Eftersom hela `0..10` behandlas likadant av systemet är det **en** ekvivalensgrupp, inte flera.
Att dela upp den i "låga", "mellan" och "höga" giltiga värden vore att blanda ihop teknikerna:
de värden som är intressanta just för att de ligger i kanten fångas redan av
gränsvärdesanalysen ovan.

De två teknikerna ersätter alltså inte varandra, utan kombineras: ekvivalenspartitionering
minskar antalet testfall i mitten, gränsvärdesanalys lägger till dem i kanterna.

---

## Gränsfall i er egen systemlogik
`system::logic::Logic`s kommandotolkning ger flera konkreta gränsfall att testa:
* **Ogiltig period.** Vad händer vid `"period 0"`? Vid ett negativt eller icke-numeriskt värde?
  Kravspecifikationen säger inget om detta, vilket gör det till ett medvetet designbeslut ni
  behöver testa och dokumentera, inte bara hoppas fungerar.
* **Okänt kommando.** Vad händer om `Logic` tar emot en rad som inte matchar något av de kända
  kommandona (`on`, `off`, `blink on/off`, `period`, `status`, `temp`)?
* **Upprepade tillståndsbyten.** Vad händer vid `"blink on"` två gånger i rad? Vid `"off"` när
  LED:en redan är släckt?
* **Exakt vid periodgränsen.** Triggar blinktoggling exakt när `period_ms` har passerat, snarare
  än en tick för tidigt eller för sent (jämför med er timer-stubbtestning i **L04**)?

---
