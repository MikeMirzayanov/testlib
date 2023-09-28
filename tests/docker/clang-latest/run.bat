@echo off

SET IMAGE_NAME=test-testlib-clang-latest
for /f "tokens=*" %%i in ('git rev-parse --abbrev-ref HEAD') do set CURRENT_BRANCH=%%i

"C:\Program Files\Docker\Docker"\DockerCli.exe -SwitchLinuxEngine
docker run -it %IMAGE_NAME% /bin/bash /startup.sh %CURRENT_BRANCH%
