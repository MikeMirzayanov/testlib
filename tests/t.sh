#!/bin/bash
set -eo pipefail

# Shortcut to run `run.sh g++ 11 v0 $*`
TESTS_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
bash $TESTS_DIR/run.sh g++ 11 v0 $*
