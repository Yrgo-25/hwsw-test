# Bilaga A - Kodkvalitet i pipelinen

## Statisk analys: buggar utan att köra koden
Statisk analys läser er källkod utan att exekvera den, och letar efter mönster som ofta är
buggar: oinitierade variabler, oanvänd kod, misstänkt komplex logik, jämförelser som alltid är
sanna eller falska. Två vanliga verktyg för C++:
* **`clang-tidy`.** Kontrollerar bland annat kodkonventioner, moderna C++-mönster och vanliga
  buggkällor. Körs t.ex. som `clang-tidy source/main.cpp -- -std=c++17 -Iinclude`.
* **`cppcheck`.** Ett fristående verktyg som främst fokuserar på att hitta potentiella buggar
  snarare än stilfrågor. Körs t.ex. som `cppcheck --enable=all --std=c++17 source/`.

Statisk analys hittar en annan typ av fel än era tester: tester verifierar att koden *gör rätt
sak* givet viss indata, statisk analys letar efter kod som är *misstänkt* oavsett indata. Alla
varningar är inte buggar; en del är falska positiver. Det viktiga är att ni tar ställning till
varje varning, och åtgärdar de som är relevanta.

---

## Branch protection: att ett rött kryss faktiskt betyder något
Som standard *visar* GitHub att jobben i er pipeline misslyckades, men hindrar ingen från att
merga ändå. Under *Settings → Branches → Add branch ruleset* för `main` kan ni kräva att
namngivna jobb har lyckats innan en pull request går att merga (*Require status checks to
pass*). Först då är pipelinen en grind och inte bara en lampa.

Jobben anges med sina id:n, alltså namnen ni själva valde i `ci.yml` i **L05** (t.ex.
`firmware-build`). Det är också den inställning som gör kravet i **P04** meningsfullt: all
utveckling sker via branches och pull requests, och det är på pull requesten kontrollen ska ske.

---

## Ordningen mellan jobben
Er pipeline har jobb för formattering och firmware-bygge sedan **L05**, och för testkörning sedan
**L06**. I **L05** kördes firmware-bygget först efter formatteringskontrollen, via `needs:`,
eftersom den ena tar sekunder och den andra minuter. Nu när testsviten vuxit är det värt att
bestämma var testjobbet hör hemma: parallellt med firmware-bygget, eller före det? Ett
statisk-analysjobb, om ni lägger till ett, står inför samma val.

Tumregeln är densamma som i **L05**: låt snabba, billiga jobb gå först när ett rött besked från
dem gör de dyra jobben meningslösa, och kör jobb parallellt när båda beskeden är intressanta
oavsett vilket som blir rött.

---
