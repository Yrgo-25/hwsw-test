/**
 * @file Hardware platform selection.
 */
#pragma once

/** Use mocked platform when testing. */
#ifdef TESTSUITE
#include "arch/env/test/hw_platform.h"
#else
/** Add ESP32-specific drivers headers here! */
#include "driver/gpio.h"
#endif /** TESTSUITE */
