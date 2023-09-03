#!/bin/bash
set -eo pipefail

pwd
ls
echo Before CMD
cmd.exe /c testme.bat
echo End CMD
