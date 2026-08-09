/**
 * @brief Hardware platform for AVR devices.
 */
#pragma once

/** When compiling for the actual AVR target, include the real AVR hardware libraries. */
#ifndef TESTSUITE

#ifndef F_CPU
#define F_CPU 16000000UL // Default CPU frequency measured in Hz.
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

/** When compiling for the test suite, include test hardware platform header instead. */
#else
#include "arch/test/hw_platform.h"
#endif
