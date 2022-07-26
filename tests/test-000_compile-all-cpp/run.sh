#!/bin/bash
set -eo pipefail

(
  IFS='
'
  for f in $(find "$TESTS_DIR"/.. -name "*.cpp"); do
    if [[ ! "$f" == *../tests/* ]]; then
      bash ../scripts/compile "$f" --check-only
    fi
  done
)
