# Testlib Project Structure - Guide for AI Agents

This document provides an overview of the testlib project structure to help AI agents understand and work with the codebase effectively.

## Project Overview

Testlib is a C++ library for competitive programming problems, providing tools for validators, checkers, generators, and interactors. It is widely used in programming competitions (Codeforces, ICPC, etc.) to prepare and validate test data.

## Core Files

- **testlib.h** - Main library header file (~207KB, contains all core functionality)
- **README.md** - Project documentation and usage instructions
- **LICENSE** - MIT License
- **read.me** - Additional documentation

## Directory Structure

### `/checkers/`
Standard checker implementations for different output comparison modes:
- **ncmp.cpp** - Token-by-token comparison (whitespace-insensitive)
- **wcmp.cpp** - Word-by-word comparison
- **lcmp.cpp** - Line-by-line comparison
- **fcmp.cpp** - Floating-point comparison with epsilon
- **rcmp.cpp, rcmp4.cpp, rcmp6.cpp, rcmp9.cpp** - Floating-point with different precision
- **dcmp.cpp** - Double comparison
- **icmp.cpp** - Integer comparison
- **hcmp.cpp** - Huge integer comparison
- **yesno.cpp, nyesno.cpp** - Yes/No answer checkers
- **acmp.cpp** - Approximate comparison
- **uncmp.cpp** - Unsigned integer comparison
- **pointscmp.cpp, pointsinfo.cpp** - Points-based comparison
- **casecmp.cpp, caseicmp.cpp, casencmp.cpp, casewcmp.cpp** - Case-sensitive/insensitive variants

### `/validators/`
Example validators for input file validation:
- **nval.cpp** - Numeric validator
- **ival.cpp** - Integer validator
- **sval.cpp** - String validator
- **case-nval.cpp** - Multi-test case validator
- **undirected-graph-validator.cpp** - Graph validator
- **undirected-tree-validator.cpp** - Tree validator
- **bipartite-graph-validator.cpp** - Bipartite graph validator
- **validate-using-testset-and-group.cpp** - Testset/group validator

### `/generators/`
Example generators for test data creation:
- **igen.cpp** - Integer generator
- **sgen.cpp** - String generator
- **bgen.cpp** - Binary generator
- **gs.cpp** - Graph generator
- **iwgen.cpp** - Integer with weights generator
- **swgen.cpp** - String with weights generator
- **multigen.cpp** - Multiple test generator
- **gen-array-with-opt.cpp** - Array generator with options
- **gen-tree-graph.cpp** - Tree generator
- **gen-rooted-tree-graph.cpp** - Rooted tree generator
- **gen-bipartite-graph.cpp** - Bipartite graph generator

### `/interactors/`
Example interactors for interactive problems:
- **interactor-a-plus-b.cpp** - Simple interactive problem example

### `/tests/`
Comprehensive test suite for testlib functionality.

## Test System

### Running Tests

Tests are executed from the `tests/` directory with various options:

```bash
# Run all tests with default settings (g++, C++11, version 0)
bash t.sh

# Run specific test
bash t.sh test-006_interactors

# Run with custom compiler
bash run.sh clang++ test-006_interactors

# Run with specific C++ standard
bash run.sh g++ 17 test-006_interactors

# Run with specific version
bash run.sh g++ 11 v1 test-006_interactors

# Run multiple tests
bash run.sh g++ 11 v0 test-001_run-sval-case-nval test-002_run-fcmp-wcmp

# Run all tests with clang++ and C++17
bash run.sh clang++ 17
```

**Command syntax**: `bash run.sh [compiler] [standard] [version] [test-names...]`
- **compiler**: `g++`, `clang++`, or `msvc` (optional, defaults to both g++ and clang++)
- **standard**: `11`, `14`, `17`, `20`, `23` (optional, defaults to all)
- **version**: `v0`, `v1`, `v2`, etc. or just `0`, `1`, `2` (optional, defaults to all)
- **test-names**: Space-separated list of test directories (optional, defaults to all tests)

**t.sh shortcut**: `bash t.sh [test-names...]` is equivalent to `bash run.sh g++ 11 v0 [test-names...]`

### Test Directory Structure

Each test follows this pattern (example: `test-006_interactors/`):

```
test-XXX_name/
├── run.sh                    # Test runner script (required)
├── src/                      # Source files for testing
│   ├── program1.cpp
│   ├── program2.cpp
│   └── ...
├── files/                    # Input data (optional)
│   ├── unix/                 # Files with LF line endings (\n)
│   │   ├── input.*
│   │   └── participant.*
│   └── win/                  # Files with CRLF line endings (\r\n)
│       ├── input.*
│       └── participant.*
└── refs/                     # Reference outputs
    └── r-test-name/          # Reference for specific test invocation
        ├── exit_code         # Expected exit code (0, 1, 2, etc.)
        ├── stdout            # Expected standard output (exact match)
        └── stderr            # Expected standard error (exact match)
```

### Test Scripts

#### `/tests/scripts/compile`

Compiles C++ source files with testlib.h.

**Environment variables required**:
- `MACHINE` - Target platform (`unix`, `Windows`)
- `CPP` - Compiler command (`g++`, `clang++`, `cl.exe`)
- `CPP_STANDARD` - Compiler standard flag (`--std=c++11`, `-std:c++17`, etc.)
- `CPP_INCLUDE_DIR` - Path to include directory (for testlib.h)
- `CPP_OPTS` - Additional compiler options (optional)

**Usage**:
```bash
bash ../scripts/compile src/program.cpp [--check-only]
```

**Features**:
- Compiles with `-Wpedantic -Werror` (GNU compilers) or equivalent
- Optimization: `-O2` by default, `-O0` with `--check-only`
- Outputs executable with same basename as source file
- On Windows, adds `.exe` extension
- Returns error if compilation fails

#### `/tests/scripts/test-ref`

Validates program execution against reference outputs.

**Environment variables required**:
- `TESTS_DIR` - Path to tests directory
- `INVOCATION_ID` - Cache key for tester-lcmp compilation

**Usage**:
```bash
bash ../scripts/test-ref <ref-name> [valgrind-options] <command...>
```

**Examples**:
```bash
# Test program execution
bash ../scripts/test-ref r-normal "$VALGRIND" ./program input.txt output.txt

# Test with stdin redirection
bash ../scripts/test-ref r-test ./program < input.txt

# Test with arguments
bash ../scripts/test-ref r-validator ./validator --testCase 1 < input.txt
```

**Behavior**:
- If reference doesn't exist: Creates reference files (exit_code, stdout, stderr)
- If reference exists: Compares actual output with reference
- Uses `tester-lcmp` for byte-perfect comparison
- Returns exit code 0 on match, 1 on mismatch
- Outputs detailed error messages showing differences

### Common Test Patterns

#### Example 1: Simple Validator Test

```bash
#!/bin/bash
set -eo pipefail

os="unix"
if [[ "$MACHINE" == "Windows" ]]; then
  os="win"
fi

bash ../scripts/compile src/validator.cpp

for input_file in files/"$os"/*.in; do
  test_name=$(basename "$input_file" .in)
  bash ../scripts/test-ref r-"$test_name" "$VALGRIND" ./validator < "$input_file"
done

rm -f validator validator.exe
```

#### Example 2: Interactor Test

```bash
#!/bin/bash
set -eo pipefail

os="unix"
if [[ "$MACHINE" == "Windows" ]]; then
  os="win"
fi

# Compile interactor
bash ../scripts/compile src/interactor.cpp

# Test with normal participant (should pass)
bash ../scripts/test-ref r-normal "$VALGRIND" ./interactor files/"$os"/input.txt output.txt < files/"$os"/participant-normal.txt

# Verify output file
tr -d '\r' < output.txt > output.nix
bash ../scripts/test-ref r-output cat output.nix

# Test with failing participant (should fail with exit code 2)
bash ../scripts/test-ref r-fail "$VALGRIND" ./interactor files/"$os"/input.txt output-fail.txt < files/"$os"/participant-fail.txt

# Cleanup
rm -f output.txt output.nix output-fail.txt interactor interactor.exe
```

#### Example 3: Multi-Version Test

```bash
#!/bin/bash
set -eo pipefail

os="unix"
if [[ "$MACHINE" == "Windows" ]]; then
  os="win"
fi

for version in v1 v2 v3; do
    bash ../scripts/compile src/"$version".cpp

    for test_num in 01 02 03; do
        bash ../scripts/test-ref "$version"-"$test_num" "$VALGRIND" ./"$version" < files/"$os"/input."$test_num"
    done

    rm -f "$version" "$version".exe
done
```

## Key Testlib Features

### InStream (Input Reading)
Used for reading from input files or streams.

**Initialization**:
```cpp
registerValidation(argc, argv);     // For validators
registerInteraction(argc, argv);    // For interactors
registerGen(argc, argv, 1);         // For generators
```

**Reading methods**:
```cpp
int n = inf.readInt(1, 100000, "n");              // Read with validation
long long x = inf.readLong();                      // Read long
double d = inf.readDouble();                       // Read double
string s = inf.readToken();                        // Read token
string line = inf.readLine();                      // Read line
char c = inf.readChar();                           // Read character
```

### OutStream (Output Checking)
Used in checkers and interactors for writing results.

**Verdict functions**:
```cpp
quitf(_ok, "Correct: %d queries processed", count);
quitf(_wa, "Wrong answer on query %d", queryNum);
quitf(_pe, "Presentation error: extra spaces");
quitf(_fail, "Checker failed: %s", error);
```

**Partial scores**:
```cpp
quitp(0.5, "Partial: solved 5 out of 10 queries");
```

### Interactor Streams
- **inf** - Input stream (test data from input file)
- **ouf** - Output stream (from participant/solution)
- **tout** - Output stream (to judge/system, for logging)
- **cout** - Standard output (goes to participant)

**Example interactor**:
```cpp
#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    registerInteraction(argc, argv);

    int n = inf.readInt();  // Read from input file

    for (int i = 0; i < n; i++) {
        int query = inf.readInt();
        cout << query << endl;  // Send to participant
        cout.flush();

        int response = ouf.readInt();  // Read from participant
        tout << response << endl;      // Log to judge
    }

    quitf(_ok, "%d queries processed", n);
}
```

### Validator Patterns

**Basic validator**:
```cpp
#include "testlib.h"

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);

    int n = inf.readInt(1, 100000, "n");
    inf.readSpace();
    int m = inf.readInt(1, 100000, "m");
    inf.readEoln();

    // Validate input...

    inf.readEof();
}
```

## Platform Differences

### Line Endings
- **Unix/Linux/macOS**: LF (`\n`, 0x0A)
- **Windows**: CRLF (`\r\n`, 0x0D 0x0A)

Store input files in both formats:
- `files/unix/` - LF line endings
- `files/win/` - CRLF line endings

Use `tr -d '\r'` to normalize Windows line endings when comparing outputs.

### Creating Files with Proper Line Endings

**Unix files**:
```bash
printf 'line1\nline2\nline3\n' > files/unix/input.txt
```

**Windows files**:
```bash
printf 'line1\r\nline2\r\nline3\r\n' > files/win/input.txt
```

## Git Workflow

- **Main branch**: `master`
- **Development**: Feature branches
- Tests must pass before merging
- Use `git rebase` to update feature branches
- Always run tests after rebase to catch changes in base branch

## Important Guidelines for AI Agents

### File Operations
1. **Always read files before modifying** - Use Read tool before Edit/Write
2. **Never create documentation files proactively** - Only when explicitly requested
3. **Preserve line endings** - Don't mix Unix/Windows line endings

### Testing
1. **Test changes locally** - Run relevant tests after modifications
2. **Create both unix and win files** - When adding new test inputs
3. **Verify reference files** - Ensure exit_code, stdout, stderr match actual behavior
4. **Clean up artifacts** - Remove generated files (executables, temp files) in run.sh

### Code Quality
1. **Use English for comments** - All code comments should be in English
2. **Compile with strict flags** - Always use `-Wpedantic -Werror`
3. **Follow existing patterns** - Look at similar tests for structure
4. **Validate before committing** - Run full test suite when possible

### Common Pitfalls
1. **Don't use `|| true` to ignore errors** - Use proper test-ref validation instead
2. **Check error messages after rebase** - Error messages may change, update reference files
3. **Reference file line endings** - Reference files should use Unix (LF) line endings
4. **MACHINE variable** - Always set via environment, not hardcoded
5. **Paths in test-ref** - Use `$os` variable for unix/win file selection

### When Adding New Tests
1. Study existing tests in the same category (validators, interactors, checkers)
2. Create both unix and win input files with appropriate line endings
3. Create reference directories with exit_code, stdout, stderr files
4. Update run.sh to compile and execute new test cases
5. Test on all supported compilers and standards if possible
6. Clean up all generated files at the end of run.sh

## Error Messages

Common testlib error exit codes:
- **0** (_ok) - Accepted/Correct
- **1** (_wa) - Wrong Answer
- **2** (_pe) - Presentation Error
- **3** (_fail) - Checker/Validator Failed
- **4** (_dirt) - Unexpected output
- **7** (_partially) - Partially Correct

## Additional Resources

- **README.md** - Main documentation
- **tests/README.md** - Test system documentation
- Each test directory may have specific documentation in comments
