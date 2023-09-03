#!/bin/bash
set -eo pipefail

pwd
ls
echo Before CMD
chmod +x testme.bat
cmd.exe /c testme.bat
echo End CMD
