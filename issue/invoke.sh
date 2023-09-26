#!/bin/bash
set -eo pipefail

exit_code=0
echo "running $* 1>out 2>err"
$* 1>out 2>err || exit_code=$?

echo exit_code="$exit_code"
echo -- out --
cat out
echo -- err --
cat err
echo -- --- --
rm -f out
rm -f err
