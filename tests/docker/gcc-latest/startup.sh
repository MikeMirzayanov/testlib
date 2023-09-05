#!/bin/bash
set -e -o pipefail

git clone https://github.com/MikeMirzayanov/testlib.git
cd testlib
git checkout dev-mikemirzayanov
cd tests
./run.sh g++ v0 23
cd /
rm -rf testlib
