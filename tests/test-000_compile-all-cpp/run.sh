#!/bin/bash
set -eo pipefail

# The examples (checkers, validators, generators, interactors) must also compile without
# shadowing diagnostics, both in their own code and in testlib.h. Other .cpp files keep the
# usual flags.
source ../scripts/shadow-flags
echo "Shadowing diagnostics for the examples: $SHADOW_PROFILE ($SHADOW_ERROR_FLAGS)"

(
  IFS='
'
  examples=0
  for f in $(find "$TESTS_DIR"/.. -name "*.cpp"); do
    if [[ ! "$f" == *../tests/* ]]; then
      if [[ "$f" == "$TESTS_DIR"/../checkers/* || "$f" == "$TESTS_DIR"/../validators/* ||
            "$f" == "$TESTS_DIR"/../generators/* || "$f" == "$TESTS_DIR"/../interactors/* ]]; then
        if [[ "$CPP" == "cl.exe" ]]; then
          CL="${CL:+$CL }$SHADOW_ERROR_FLAGS" bash ../scripts/compile "$f" --check-only
        else
          CPP_OPTS="$CPP_OPTS $SHADOW_ERROR_FLAGS" bash ../scripts/compile "$f" --check-only
        fi
        examples=$((examples + 1))
      else
        bash ../scripts/compile "$f" --check-only
      fi
    fi
  done
  echo "Compiled $examples examples with shadowing diagnostics as errors"
  if [[ "$examples" -eq 0 ]]; then
    echo "No examples found"
    exit 1
  fi
)
