# Bilaga A - Stubbar och virtuell hårdvara

## Stubb, mock eller fake?
Tre besläktade begrepp används ofta om vartannat, men betyder olika saker:
* **Stubb.** En enkel, alternativ implementation av ett interface som returnerar fördefinierade
  svar. Testet verifierar resultatet av koden som *använder* stubben, inte stubben själv. Det är
  den typ ni redan använt i **L01–L04**.
* **Spy.** Liknar en stubb, men registrerar dessutom *hur* den anropades (vilka funktioner, med
  vilka argument, hur många gånger). **Testet** läser av det i efterhand och verifierar själva
  interaktionen. Det är den varianten ni bygger nedan, via `gpio_last_config()`.
* **Mock.** Får sina förväntningar i förväg ("`gpio_set_level` ska anropas exakt en gång med
  pin 5") och failar *själv* vid ett oväntat anrop, i stället för att testet kontrollerar
  efteråt. `yrgo::test` har inget separat mock-stöd.
* **Fake.** En förenklad men fungerande implementation, t.ex. en in-memory-databas i stället för
  en riktig. Mer komplex än en stubb, men fortfarande betydligt enklare än den riktiga
  implementationen.
* **Dummy.** Fyller bara en parameterplats för att koden ska gå att anropa, och används aldrig.

I dagligt tal kallas ofta både spy och mock för "mock", och det gör vi också i den här kursen
när skillnaden inte spelar roll. Var ändå medvetna om distinktionen: den kan dyka upp som en
tentafråga.

---

## Två nivåer att hålla isär
Era **P02**-drivers rör aldrig hårdvaruregister direkt: de anropar i stället ESP-IDF:s egna
C-funktioner (t.ex. `gpio_set_level()`, `gpio_get_level()`, `uart_write_bytes()`), som i sin tur
sköter registerkommunikationen internt i ESP-IDF. Det ger två olika nivåer att testa på, med två
olika tekniker:
1. **Stubba driverns eget interface**: en klass som ärver t.ex. `driver::gpio::Interface` och
   används i stället för den riktiga drivern av annan kod, t.ex. systemlogiken. Ni byggde redan
   dessa stubbar i **P02** (`driver::gpio::Stub` med flera); att skriva enhetstester för dem är
   en del av **P04**, med samma teknik som ni övat på i **L02–L04**. Den här tekniken används
   för att **komponenttesta** kod som *beror på* en driver (se **L08**), inte för att testa
   drivern själv.
2. **Mocka ESP-IDF:s C-funktioner**: eftersom driverklassen internt anropar ESP-IDF:s C-API,
   krävs en annan teknik för att kunna **enhetstesta driverklassen själv** på värddatorn. Det är
   den tekniken som beskrivs nedan.

---

## Att mocka ESP-IDF:s C-funktioner
ESP-IDF:s funktioner har C-länkning (`extern "C"`) med en fast, dokumenterad signatur. Länkaren
matchar bara **symbolnamnet**, inte signaturen, som inte finns kvar i objektfilen efter
kompileringen. Det innebär att ni kan skriva en egen implementation av samma funktioner och
länka in den i stället för ESP-IDF:s riktiga när testsviten byggs. Det innebär också att det är
ert eget ansvar att deklarationen i mock-headern är identisk med ESP-IDF:s: en avvikande
signatur upptäcks *inte* av länkaren, utan ger fel först vid körning.

Nedan visas ett exempel på en enkel mock av två ESP-IDF-funktioner för tester via en fil döpt 
`test/esp/gpio_mock.h`:

```c
#ifndef ESP_GPIO_MOCK_H_
#define ESP_GPIO_MOCK_H_

#include <stdint.h>

/** Error code indicating success. */
#define ESP_OK 0

/** Error code indicating invalid argument. */
#define ESP_ERR_INVALID_ARG -1

/** Alias for ESP error codes. */
typedef int esp_err_t;

/** Alias for GPIO pins. */
typedef uint8_t gpio_num_t;

/**
 * @brief Set GPIO level.
 * 
 * @param[in] pin GPIO pin to set.
 * @param[in] level GPIO level (1 = enabled, 0 = disabled).
 * 
 * @return ESP_OK on success, ESP_ERR_INVALID_ARG if the pin is invalid.
 */
esp_err_t gpio_set_level(gpio_num_t pin, uint32_t level);

/**
 * @brief Get GPIO level.
 * 
 * @param[in] pin GPIO pin to read.
 * 
 * @return 1 if enabled, 0 if disabled.
 * 
 * @note No pin validation is done; 0 is returned if the pin is invalid.
 */
int gpio_get_level(const gpio_num_t pin);

#endif /** ESP_GPIO_MOCK_H_ */
```

Motsvarande källkodsfil `test/esp/gpio_mock.c` hade kunnat implementerats såsom visas nedan:

```c
#include <stdbool.h>
#include <stdint.h>

#include "test/esp/gpio_mock.h"

/** Maximum valid pin number for ESP32-S3. */
#define PIN_MAX 48U

/** PIN registry (1 = enabled, 0 = disabled). */
static uint64_t pin_reg = 0U;

// -----------------------------------------------------------------------------
static inline bool is_pin_valid(const gpio_num_t pin) { return PIN_MAX >= pin; }

// -----------------------------------------------------------------------------
esp_err_t gpio_set_level(const gpio_num_t pin, const uint32_t level)
{
    // Check the pin number, return error if invalid.
    if (!is_pin_valid(pin)) { return ESP_ERR_INVALID_ARG; }

    // Set new GPIO level, then return true to indicate success.
    if (0U != level)
    {
        pin_reg |= (1ULL << pin);
    }
    else
    {
        pin_reg &= ~(1ULL << pin);
    }
    return ESP_OK;
}

// -----------------------------------------------------------------------------
int gpio_get_level(const gpio_num_t pin)
{
    // Read the GPIO level, return 1 if enabled, 0 if disabled.
    //! @note gpio_get_level() does not validate the pin, unlike gpio_set_level().
    return (pin_reg & (1ULL << pin)) ? 1 : 0;
}
```

Er driverklass (t.ex. `driver::gpio::Esp32s3`) behöver inte ändras alls, den anropar exakt samma
funktioner som förut. Skillnaden ligger i **vad testsvitens Makefile länkar mot**: ESP-IDF:s
riktiga gpio-komponent byts ut mot er egen `test/esp/gpio_mock.c`. `pin_reg` är `static`, alltså
inte synlig utanför den här filen, men det behövs inte heller: ett test verifierar vad
`gpio_set_level()` satte genom att helt enkelt anropa `gpio_get_level()` för samma pin, precis
som drivern själv gör.

---

## Mocka funktioner med struktar som argument
Vissa ESP-IDF-funktioner tar emot ett helt konfigurations-strukt i stället för enstaka
argument, t.ex. `gpio_config(const gpio_config_t* config)`, som en del av era drivers anropar i
konstruktorn för att sätta upp riktning, pull-up/pull-down samt avbrottstyp i ett enda anrop.
Samma princip gäller som ovan, med ett par tillägg:
* Lägg till en egen, minimal version av strukten i mock-headern, med samma fältnamn
  (medlemsvariabler) som originalet; lägg bara till de fält ni faktiskt använder i er driver.
* Låt mock-implementationen spara en **kopia** av den senaste GPIO-konfigurationen, inte en
  pekare till den: `config` pekar normalt på en lokal variabel i den anropande funktionen (er
  GPIO-konstruktor), och är inte längre giltig efter att anropet returnerat.
* Lägg till en publik funktion som returnerar en `const`-pekare till den sparade
  GPIO-konfigurationen, för validering i era enhetstester. Det är säkert att returnera en
  pekare just här, eftersom den pekar på en `static`-variabel med statisk lagringstid som lever
  kvar efter att funktionen returnerat, till skillnad från den ursprungliga `config`-pekaren.

Så här kan tillägget till headerfilen `test/esp/gpio_mock.h` se ut:

```c
/** 
 * @brief GPIO config structure.
 */
typedef struct
{
    /** Pin bit mask. */
    uint64_t pin_bit_mask;

    /** GPIO mode (direction). */
    int mode;

    /** Pullup enablement flag. */
    int pull_up_en;

    /** Pulldown enablement flag. */
    int pull_down_en;

    /** Interrupt type. */
    int intr_type;
} gpio_config_t;

/**
 * @brief Configure GPIO.
 * 
 * @param[in] config GPIO configuration.
 * 
 * @return ESP_OK on success, ESP_ERR_INVALID_ARG if the configuration is invalid.
 */
esp_err_t gpio_config(const gpio_config_t* config);


/**
 * @brief Get the last configuration passed to gpio_config().
 * 
 * @return Pointer to the last configuration passed to gpio_config().
 */
const gpio_config_t* gpio_last_config(void);
```


Och så här kan motsvarande tillägg till `test/esp/gpio_mock.c` se ut, med en filglobal
struktinstans som lagrar en kopia av den senaste passerade konfigurationen:

```c
#include <stddef.h>

/** Last GPIO configuration passed to gpio_config(). */
static gpio_config_t last_config;

// -----------------------------------------------------------------------------
esp_err_t gpio_config(const gpio_config_t* config)
{
    // Check the configuration, return error if invalid.
    if (NULL == config) { return ESP_ERR_INVALID_ARG; }

    // Store a copy of the configuration, then return ESP_OK to indicate success.
    last_config = *config;
    return ESP_OK;
}

// -----------------------------------------------------------------------------
const gpio_config_t* gpio_last_config(void) { return &last_config; }
```

Ett test kan sedan verifiera att drivern anropade `gpio_config()` med rätt `pin_bit_mask` samt
`mode`, genom att läsa av `gpio_last_config()`. Till skillnad från `gpio_set_level()`, som kan
verifieras via sin egen motpart `gpio_get_level()`, har `gpio_config()` inget sådant
motsvarande läsanrop i den riktiga ESP-IDF-API:n, så en egen accessor-funktion i mocken behövs
här för att kunna verifiera anropet över huvud taget. Bara de av era drivers som faktiskt
anropar en strukt-baserad funktion behöver den här tekniken, flera klarar sig med enbart
enstaka argument, som i föregående exempel.

---

## Tips: en delad `arch/`-mapp för att välja header
Byggsystemet behöver inte bara byta ut vilken källkodsfil som länkas in, det kan även styra
**vilken header** som inkluderas, utan att ändra en enda `#include`-rad i drivern. Lägg era
mock-headers i en egen mapp, t.ex. `test/arch/`, med samma filnamn som originalen (t.ex.
`test/arch/driver/gpio.h` i stället för ESP-IDF:s riktiga `driver/gpio.h`). Genom att lägga
`test/arch/` **först** i kompilatorns sökväg (`-I`) när testsviten byggs, till exempel styrt av
en egen flagga (jämför med `TESTSUITE`), hittar kompilatorn er mock-header i stället för
ESP-IDF:s riktiga, trots att drivern fortfarande bara skriver `#include "driver/gpio.h"`.

Målet är samma som för `include/arch/avr/hw_platform.h` respektive
`include/arch/test/hw_platform.h` i det delade
[ATmega328p-övningsbiblioteket](../../../libs/atmega/README.md) (se **L02–L04**): produktionskoden
ska förbli oförändrad oavsett vilken miljö testsviten byggs för. Mekaniken skiljer sig dock åt.
`hw_platform.h` är en egen, projektägd header, så den kan innehålla en `#ifdef TESTSUITE` som
själv väljer vilken variant som inkluderas. `driver/gpio.h` däremot är ESP-IDF:s egen header,
den går inte att lägga en `#ifdef` i. Därför måste valet i stället ske i kompilatorns sökväg,
genom att er egen mock-mapp läggs före ESP-IDF:s riktiga headerkatalog.

---
