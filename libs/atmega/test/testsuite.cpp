/**
 * @brief Test suite for the libatmega library.
 */
#include "yrgo/test/test.h"

/**
 * @brief Run test suite.
 *
 * @return 0 on success, or -1 on failure.
 */
int main() { return yrgo::test::runAllTests() ? 0 : -1; }
