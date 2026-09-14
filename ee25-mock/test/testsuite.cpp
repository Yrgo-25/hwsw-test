#include <cstdio>

#include "driver/gpio/esp32.h"

int main()
{
    driver::gpio::Esp32 led1{9U};
    const auto state1 = led1.read();
    std::printf("GPIO state: %s\n", state1 ? "on" : "off");

    led1.write(true);
    const auto state2 = led1.read();
    std::printf("GPIO state: %s\n", state2 ? "on" : "off");
    return 0;
}
