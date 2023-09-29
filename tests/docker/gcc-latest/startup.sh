#!/bin/bash
set -e -o pipefail

CPP_STANDARD=23

BRANCH=${1:-dev-mikemirzayanov}
echo "Use branch ""$BRANCH"""

git clone https://github.com/MikeMirzayanov/testlib.git
cd testlib
git checkout "$BRANCH"
cd tests
./run.sh g++ v0 "$CPP_STANDARD"
cd /
rm -rf testlib
