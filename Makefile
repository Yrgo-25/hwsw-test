# Build the yrgo::test framework and the library test suites.
build:
	@bash ci/build.sh

# Format all C/C++ files (clang-format) and Python files (black) in place.
format:
	@bash ci/format.sh

# Check formatting without modifying any files; fails if something isn't formatted.
format-check:
	@bash ci/format.sh --check

.PHONY: build format format-check
