# Bilaga B - Övningsuppgifter
Ni har redan hårdvara och en egen kodbas från **P02** sedan tidigare, men dessa
övningsuppgifter görs medvetet utan att röra vare sig hårdvaran eller er egen kodbas. Syftet är
att öva på själva tekniken i ett litet, isolerat format innan ni tillämpar den på riktigt.

Ni ska skapa två små, självständiga stubbdrivers som efterliknar hårdvara med vanliga
medlemsvariabler. Ni ska sedan skriva enhetstester för dessa. Samma princip använder ni sedan
mot era egna stubbar från **P02** i **L02–L04**.

**OBS!** Namnrymden `test::driver::` används medvetet nedan, i stället för `driver::`.
Interfacen ni bygger här är kraftigt förenklade övningsvarianter, och har alltså *inte* samma
metoder som de riktiga `driver::gpio::Interface` och `driver::timer::Interface` som ni möter i
övningsbiblioteket och i **P02**. Det är principen, inte API:et, som ska kännas igen. De
riktiga `Esp32s3`-drivers, som anropar ESP-IDF, kräver i stället mockningstekniken från **L06**
för att kunna enhetstestas, något ni tillämpar fullt ut när ni skriver tester för er riktiga
kodbas i **P04**.

Skapa en liten mapp för övningen (valfritt namn, t.ex. `unit_test_exercise/`) med en egen Makefile
som bygger mot `yrgo::test` (se exemplet i 
[libs/test](../../../libs/test/README.md#integration-as-a-git-submodule); i den här
övningsmappen blir sökvägen `YRGO_TEST_DIR := ../libs/test`, se
[libs/atmega/test/makefile](../../../libs/atmega/test/makefile) för ett fungerande exempel
i det här repot).

---

## Uppgift 1 - GPIO-stubb

### Skapa interfacet och stubben
Skapa ett interface `test::driver::gpio::Interface` med metoderna:
* `write(bool high)`: sätter pinnens logiska nivå.
* `read() const`: läser pinnens nuvarande logiska nivå.
* `toggle()`: togglar pinnens nuvarande logiska nivå.

Skapa därefter en klass `test::driver::gpio::Stub` som ärver interfacet. I stället för att
använda en riktig pin lagrar stubben bara ett bool-värde internt (starttillstånd valfritt,
t.ex. `false` som default via konstruktorn).

### Skriv enhetstester
Skriv enhetstester (`TEST(GpioStub, ...)`) som verifierar bland annat:
* Att en nyskapad stubb har rätt starttillstånd.
* Att `write(true)` respektive `write(false)` sätter pinnen till rätt nivå.
* Att `toggle()` växlar tillståndet, och att två `toggle()`-anrop i rad återställer det
  ursprungliga tillståndet.

---

## Uppgift 2 - Timer-stubb

### Skapa interfacet och stubben
Skapa ett interface `test::driver::timer::Interface` med metoderna:
* `reset()`: nollställer timern och börjar räkna förfluten tid från och med nu.
* `elapsedMs() const`: returnerar antal millisekunder som förflutit sedan senaste `reset()`.
* `hasElapsed(std::uint32_t intervalMs) const`: returnerar `true` om minst `intervalMs`
  millisekunder har förflutit sedan senaste `reset()`.

Skapa därefter en klass `test::driver::timer::Stub` som ärver interfacet:
* Eftersom vi varken vill eller kan vänta in riktig tid i ett enhetstest ska stubben inte mäta
  verklig tid.
* Lägg i stället till en extra metod `tick()`, som bara finns i stubben. Denna metod ska flytta
  stubbens virtuella klocka framåt ett steg (t.ex. 1 ms), likt en riktig timer-interrupt som
  fyrar med jämna mellanrum.
* `elapsedMs()` returnerar då bara antalet `tick()`-anrop (omräknat till millisekunder) som
  gjorts sedan senaste `reset()`. Vill ett test simulera att t.ex. 100 ms har förflutit anropas
  `tick()` 100 gånger i en loop.

### Skriv enhetstester
Skriv enhetstester (`TEST(TimerStub, ...)`) som verifierar bland annat:
* Att en nyskapad (eller nyss nollställd) timer har `elapsedMs() == 0` och att `hasElapsed()`
  därför är falskt för alla positiva intervall.
* Att upprepade `tick()`-anrop faktiskt ökar `elapsedMs()` med rätt antal millisekunder.
* Att `hasElapsed(intervalMs)` blir sant när den virtuella klockan når exakt `intervalMs`, men
  är falskt strax innan.
* Att `reset()` nollställer `elapsedMs()` igen, även efter flera `tick()`-anrop.

---
