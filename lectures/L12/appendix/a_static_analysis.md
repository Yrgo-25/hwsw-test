# Bilaga A - Statisk analys och sanitizers

## Statisk analys: buggar utan att köra koden
Statisk analys läser er källkod utan att exekvera den, och letar efter mönster som ofta är
buggar: oinitierade variabler, oanvänd kod, misstänkt komplex logik, jämförelser som alltid är
sanna eller falska. Två vanliga verktyg för C++:
* **`clang-tidy`.** Kontrollerar bland annat kodkonventioner, moderna C++-mönster och vanliga
  buggkällor. Körs t.ex. som `clang-tidy source/main.cpp -- -std=c++17 -Iinclude`.
* **`cppcheck`.** Ett fristående verktyg som främst fokuserar på att hitta potentiella buggar snarare än stilfrågor. Körs t.ex. som `cppcheck --enable=all --std=c++17 source/`.

Statisk analys hittar en annan typ av fel än era tester: tester verifierar att koden *gör rätt
sak* givet viss indata, statisk analys letar efter kod som är *misstänkt* oavsett indata.

---

## Sanitizers: buggar som bara syns vid körning
**AddressSanitizer (ASan)** och **UndefinedBehaviorSanitizer (UBSan)** är körtidsverktyg som
lägger in extra kontroller i programmet vid kompilering och rapporterar felet där det uppstår, i
stället för att låta det tysta felet fortsätta orsaka problem längre fram:
* **ASan** hittar bland annat minnesfel: läsning/skrivning utanför en arrays gränser, användning
  av redan frigjort minne.
* **UBSan** hittar odefinierat beteende: teckenrelaterat heltalsoverflow (`signed`), delning med
  noll, felaktiga typkonverteringar. Notera att wraparound för `unsigned` är väldefinierat i
  C++ och därför *inte* rapporteras.

Aktiveras vid kompilering, t.ex.:
```bash
g++ -std=c++17 -fsanitize=address,undefined -fno-sanitize-recover=all \
    -fno-omit-frame-pointer -g source/main.cpp -o app
```

`-fno-sanitize-recover=all` är viktig: ASan avbryter programmet av sig själv, men UBSan skriver
som standard bara ut `runtime error: ...` och låter programmet fortsätta *med oförändrad
felkod*. Utan flaggan kan alltså testsviten, och därmed CI-jobbet, lysa grönt trots att UBSan
hittat odefinierat beteende. `-fno-omit-frame-pointer` ger läsbara anropsstackar.

Om ni kompilerar och länkar i separata steg måste `-fsanitize=...` anges i **båda** stegen,
annars misslyckas länkningen med odefinierade `__asan_*`-symboler. I övningsbibliotekets
testsvit räcker det med:
```bash
cd libs/atmega/test
make clean && make EXTRA_FLAGS="-fsanitize=address,undefined -fno-sanitize-recover=all -g"
```

Kör sedan programmet (eller testsviten) som vanligt. Sanitizers är särskilt värdefulla att köra
mot just testsviten, eftersom testerna redan motionerar koden med varierad indata.

Utan en sanitizer kan samma fel orsaka en krasch långt senare, eller ge felaktiga resultat utan
att programmet kraschar alls. En sanitizer rapporterar i stället felet direkt där det uppstår,
med en detaljerad felrapport, och avbryter programmet med en nollskild felkod så länge
`-fno-sanitize-recover=all` är angiven.

I många projekt körs både statisk analys och sanitizers automatiskt i CI vid varje commit.

---
