/**
 * @file Hardware platform selection.
 */
#pragma once

/** Use mocked platform when testing. */
#ifdef TESTSUITE
#include "arch/env/test/hw_platform.h"
#else
// ESP32 GPIO-driver.
#include "driver/gpio.h"
#endif /** TESTSUITE */
