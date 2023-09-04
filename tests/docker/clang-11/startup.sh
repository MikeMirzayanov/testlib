#!/bin/bash
set -e -o pipefail

git clone https://github.com/MikeMirzayanov/testlib.git
cd testlib
git checkout dev-mikemirzayanov
cd tests
./run.sh v0 11
cd /
rm -rf testlib
