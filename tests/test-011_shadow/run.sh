#!/bin/bash
set -eo pipefail

# testlib.h must not produce shadowing diagnostics (g++ -Wshadow, clang++ -Wshadow-all,
# MSVC C4456-C4459), while shadowing in the program's own code is still diagnosed.

source ../scripts/shadow-flags
echo "Shadowing diagnostics: $SHADOW_PROFILE, errors: $SHADOW_ERROR_FLAGS, warnings: $SHADOW_WARN_FLAGS"

if [[ "$SHADOW_PROFILE" == "clang-base" ]]; then
  echo "[INFO] -Wshadow-all is not supported by $CPP, using -Wshadow"
fi

# Syntax-only compilation of $1 with extra flags $2 (word-split), include directory $3.
# Prints the compiler output, returns its exit status.
check() {
  local src=$1 flags=$2 include_dir=$3
  if [[ "$CPP" == "cl.exe" ]]; then
    echo "Running:" "$CPP" -nologo "$CPP_STANDARD" -EHsc -Zs $flags -I"$include_dir" "$src"
    "$CPP" -nologo "$CPP_STANDARD" -EHsc -Zs $flags -I"$include_dir" "$src" 2>&1
  else
    echo "Running:" "$CPP" $CPP_OPTS "$CPP_STANDARD" -fsyntax-only $flags -I"$include_dir" "$src"
    "$CPP" $CPP_OPTS "$CPP_STANDARD" -fsyntax-only $flags -I"$include_dir" "$src" 2>&1
  fi
}

# testlib.h itself: the probes must compile with shadowing diagnostics as errors.
expect_clean() {
  local src=$1 include_dir=$2 output status=0
  output=$(check "$src" "$SHADOW_ERROR_FLAGS" "$include_dir") || status=$?
  echo "$output"
  if [[ "$status" -ne 0 ]]; then
    echo -e "${RED}[FAILED]${NC} $src: shadowing diagnostics (exit code $status)"
    exit 1
  fi
  echo -e "${GREEN}[SUCCESS]${NC} $src: no shadowing diagnostics"
}

# A program's own shadowing after #include "testlib.h": $2 is the expected diagnostic
# (a regular expression matched after the file location) or "none".
expect_diagnosed() {
  local src=$1 expected=$2 name output status=0
  name=$(basename "$src")
  if [[ "$expected" == "none" ]]; then
    output=$(check "$src" "$SHADOW_ERROR_FLAGS" "$CPP_INCLUDE_DIR") || status=$?
    echo "$output"
    if [[ "$status" -ne 0 ]]; then
      echo -e "${RED}[FAILED]${NC} $name: expected no diagnostic (exit code $status)"
      exit 1
    fi
    echo -e "${GREEN}[SUCCESS]${NC} $name: not diagnosed, as expected"
    return
  fi

  # As errors: compilation fails, and because of the expected diagnostic in this file.
  output=$(check "$src" "$SHADOW_ERROR_FLAGS" "$CPP_INCLUDE_DIR") || status=$?
  echo "$output"
  if [[ "$status" -eq 0 ]]; then
    echo -e "${RED}[FAILED]${NC} $name: expected a shadowing error, but it compiled"
    exit 1
  fi
  if ! echo "$output" | grep -qE "$name[:(][0-9].*$expected"; then
    echo -e "${RED}[FAILED]${NC} $name: compilation failed without the expected diagnostic /$expected/"
    exit 1
  fi
  if echo "$output" | grep -qE "testlib\.h[:(][0-9]"; then
    echo -e "${RED}[FAILED]${NC} $name: diagnostics in testlib.h"
    exit 1
  fi

  # As warnings: the same file compiles and the warning is reported, so the failure above was
  # not caused by anything else.
  status=0
  output=$(check "$src" "$SHADOW_WARN_FLAGS" "$CPP_INCLUDE_DIR") || status=$?
  echo "$output"
  if [[ "$status" -ne 0 ]]; then
    echo -e "${RED}[FAILED]${NC} $name: does not compile with shadowing diagnostics as warnings"
    exit 1
  fi
  if ! echo "$output" | grep -qE "$name[:(][0-9].*$expected"; then
    echo -e "${RED}[FAILED]${NC} $name: no warning /$expected/"
    exit 1
  fi
  echo -e "${GREEN}[SUCCESS]${NC} $name: diagnosed as expected"
}

for probe in files/shadow-probe.cpp files/shadow-probe-exit-exception.cpp files/shadow-api.cpp; do
  expect_clean "$probe" "$CPP_INCLUDE_DIR"
done

# A program's globals named like locals of testlib templates: MSVC checks template bodies where
# they are instantiated, after these globals, so the locals carry a "_" prefix.
expect_clean files/fixture-user-globals.cpp "$CPP_INCLUDE_DIR"
# The "_x" names of those locals are not macros.
expect_clean files/shadow-names.cpp "$CPP_INCLUDE_DIR"

if [[ "$SHADOW_PROFILE" == "msvc" ]]; then
  # testlib.h disables C4458 for the rest of the translation unit. Check testlib.h itself against
  # C4458 with a copy without that one line.
  pragma='#       pragma warning( disable : 4458 )'
  if [[ "$(grep -cxF "$pragma" "$CPP_INCLUDE_DIR"/testlib.h)" != "1" ]]; then
    echo -e "${RED}[FAILED]${NC} expected exactly one line '$pragma' in testlib.h"
    exit 1
  fi
  rm -rf shadow-audit
  mkdir shadow-audit
  grep -vxF "$pragma" "$CPP_INCLUDE_DIR"/testlib.h > shadow-audit/testlib.h
  if [[ "$(($(wc -l < "$CPP_INCLUDE_DIR"/testlib.h) - $(wc -l < shadow-audit/testlib.h)))" != "1" ]]; then
    echo -e "${RED}[FAILED]${NC} the audit copy of testlib.h must differ by exactly one line"
    exit 1
  fi
  for probe in files/shadow-probe.cpp files/shadow-probe-exit-exception.cpp files/shadow-api.cpp; do
    expect_clean "$probe" shadow-audit
  done
  rm -rf shadow-audit

  expect_diagnosed files/fixture-global.cpp 'C4459'
  expect_diagnosed files/fixture-local.cpp 'C4456'
  expect_diagnosed files/fixture-parameter.cpp 'C4457'
  expect_diagnosed files/fixture-member.cpp none
  expect_diagnosed files/fixture-constructor.cpp none
else
  expect_diagnosed files/fixture-global.cpp '-W[a-z=,-]*shadow'
  expect_diagnosed files/fixture-local.cpp '-W[a-z=,-]*shadow'
  expect_diagnosed files/fixture-parameter.cpp '-W[a-z=,-]*shadow'
  expect_diagnosed files/fixture-member.cpp '-W[a-z=,-]*shadow'
  if [[ "$SHADOW_PROFILE" == "clang-base" ]]; then
    echo "[INFO] fixture-constructor.cpp is not checked with -Wshadow of $CPP"
  else
    expect_diagnosed files/fixture-constructor.cpp '-W[a-z=,-]*shadow'
  fi
fi
