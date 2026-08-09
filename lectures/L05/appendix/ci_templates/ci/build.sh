#!/usr/bin/env bash
#
# Build the yrgo::test framework and run its bundled example, to verify that
# the submodule in libs/test is intact and buildable. Also build and run the
# ATmega328p practice library's test suite in libs/atmega, to verify that it
# still builds and that its already-complete tests still pass.
#
# Usage:
#   ci/build.sh
set -euo pipefail

# Root directory (resolved to an absolute path, since this script itself cd's around).
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

# Test framework directory.
TEST_LIB_DIR="$ROOT_DIR/libs/test"

# ATmega328p practice library test directory.
ATMEGA_TEST_DIR="$ROOT_DIR/libs/atmega/test"

# Build the yrgo::test library and run the bundled example.
cd "$TEST_LIB_DIR"
make

# Build and run the ATmega328p practice library's test suite.
cd "$ATMEGA_TEST_DIR"
make
