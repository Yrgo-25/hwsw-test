# Bilaga B - Robust mjukvara och sanitizers

## Säker och robust mjukvara
Ett system som styr fysisk hårdvara bör hantera ogiltig indata och oväntade tillstånd på ett
kontrollerat sätt, i stället för att fortsätta i ett odefinierat tillstånd. Några principer:
* **Validera indata vid systemets gränser**, t.ex. värden som kommer från en sensor eller från
  seriell kommunikation, snarare än att lita blint på att de alltid är rimliga.
* **Markera funktioner `noexcept`** där de rimligen inte ska kasta undantag, och undvik
  undantag (`throw`/`try`/`catch`) i hårdvarunära kod överlag, i linje med kursens kodbaser.
* **Skilj på ogiltig konfiguration och ogiltig indata.**
  * Ett objekt som inte går att skapa korrekt, t.ex. en konstruktor som får en ogiltig pin eller
    storlek 0, bör faila högt: ett tydligt felmeddelande och ett kontrollerat avslut
    (`std::terminate()`), i stället för att fortsätta i ett odefinierat läge.
  * Ogiltig indata till ett objekt som redan fungerar, t.ex. ett okänt serial-kommando eller en
    matris med fel dimension, bör i stället avvisas med ett returvärde (`false`), och objektet
    ska behålla sitt tidigare, giltiga tillstånd. Det gör beteendet testbart: ett
    `std::terminate()` avslutar hela testprogrammet, så `yrgo::test` hinner aldrig rapportera
    felet, och inga efterföljande testfall körs.
* **Kontrollera returvärden.** Ignorera inte felkoder från hårdvara eller operativsystem. Ett
  misslyckat anrop bör hanteras direkt, inte upptäckas långt senare.

Varje sådan princip i produktionskoden är också ett gränsfall att testa: vad *ska* `Logic` göra
vid `"period 0"`, ett okänt kommando eller `"off"` när LED:en redan är släckt? Se listan över
gränsfall i [bilaga A](./a_boundary_analysis.md#gränsfall-i-er-egen-systemlogik).

---

## Samtidiga händelser
En typ av gränsfall som sällan dyker upp av sig själv i ett test är **samtidiga händelser**: två
saker som inträffar inom samma iteration av `Logic`s loop. I ett komponenttest med stubbar går
de att framkalla exakt, till skillnad från på riktig hårdvara:
* Ett `"blink off"`-kommando i `driver::serial::Stub` samtidigt som `driver::timer::Stub`s
  timeout triggas. Är LED:en släckt efteråt, oavsett i vilken ordning `Logic` hanterar dem?
* Ett `"period"`-kommando som kommer precis när den gamla perioden löper ut. Används den nya
  perioden från och med nästa toggling, eller blir en toggling kvar med den gamla?

Testet sätter båda händelserna via stubbarna innan `Logic` körs ett steg, och verifierar sedan
det förväntade tillståndet.

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
annars misslyckas länkningen med odefinierade `__asan_*`-symboler.

Kör sedan testsviten som vanligt. Sanitizers är särskilt värdefulla att köra mot just
testsviten, eftersom testerna redan motionerar koden med varierad indata, inte minst de gränsfall
ni skriver under den här lektionen.

---
