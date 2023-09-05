#!/bin/bash
set -e -o pipefail

echo "Checking installed Java"
java -version

TESTS_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
export TESTS_DIR="$TESTS_DIR"

export RED='\033[0;31m'
export CYAN='\033[1;36m'
export BLUE='\033[1;34m'
export YELLOW='\033[1;33m'
export GREEN='\033[1;32m'
export NC='\033[0m'

ARGS_CPP=""
ARGS_VALID_CPP_STANDARDS=",11,14,17,20,23,"
ARGS_CPP_STANDARDS=","
ARGS_CPP_VERSIONS=","
ARGS_TESTS=","
ARGS_CPP_BITS=""

MSVC_INCLUDE_YEAR="2022"
WINKIT_INCLUDE_VERSION="10.0.19041.0"

for arg in "$@"; do
  if [[ "$arg" == test-* ]]; then
    ARGS_TESTS="${ARGS_TESTS}${arg},"
    continue
  fi
  if [[ "$arg" == "32" || "$arg" == "64" ]]; then
    if [[ -n "$ARGS_CPP_BITS" ]]; then
      echo Expected at most one bits-argument: 32 or 64
      exit 1
    fi
    ARGS_CPP_BITS="$arg"
    continue
  fi
  if [[ "$ARGS_VALID_CPP_STANDARDS" == *,$arg,* ]]; then
    ARGS_CPP_STANDARDS="$ARGS_CPP_STANDARDS$arg,"
  else
    if [[ "$arg" == v* ]]; then
      ARGS_CPP_VERSIONS="$ARGS_CPP_VERSIONS${arg:1},"
    else
      if [[ -n "$ARGS_CPP" ]]; then
        echo Expected just one non-version argument: g++, clang++ or msvc, but found "$ARGS_CPP" and "$arg"
        exit 1
      fi
      if [[ ! "$arg" == "g++" && ! "$arg" == "clang++" && ! "$arg" == "msvc" ]]; then
        echo Expected just one non-version argument: g++, clang++ or msvc, but "$arg" found
        exit 1
      fi
      ARGS_CPP="$arg"
    fi
  fi
done

if [[ ! "$ARGS_TESTS" == "," ]]; then
  echo [INFO] Filter tests: ${ARGS_TESTS:1:${#ARGS_TESTS}-2}
fi

if [[ ! "$ARGS_CPP" == "" ]]; then
  echo [INFO] Filter CPP: "$ARGS_CPP"
fi

if [[ ! "$ARGS_CPP_STANDARDS" == "," ]]; then
  echo [INFO] Filter standards: ${ARGS_CPP_STANDARDS:1:${#ARGS_CPP_STANDARDS}-2}
fi

if [[ ! "$ARGS_CPP_VERSIONS" == "," ]]; then
  echo [INFO] Filter versions: ${ARGS_CPP_VERSIONS:1:${#ARGS_CPP_VERSIONS}-2}
fi

if [[ ! "$ARGS_CPP_BITS" == "" ]]; then
  echo [INFO] Filter target arch.: "$ARGS_CPP_BITS"
fi

COMPILERS=('clang++' 'g++')
if [[ ! "$ARGS_CPP" == "" ]]; then
  if [[ "$ARGS_CPP" == "msvc" ]]; then
    COMPILERS=()
  else
    COMPILERS=("$ARGS_CPP")
  fi
fi
echo ""
echo GNU-like compilers:
printf '    %s\n' "${COMPILERS[@]}"

CPP_STANDARDS=()
MSVC_CPP_STANDARDS=()
for v in 11 14 17 20 23; do
  if [[ "$ARGS_CPP_STANDARDS" == "," || "$ARGS_CPP_STANDARDS" == *,$v,* ]]; then
    CPP_STANDARDS+=("--std=c++$v")
    MSVC_CPP_STANDARDS+=("-std:c++$v")
  fi
done
echo ""
echo Standards:
printf '    %s\n' "${CPP_STANDARDS[@]}"

uname_output="$(uname -s)"
case "${uname_output}" in
Linux*) machine=Linux ;;
Darwin*) machine=Mac ;;
CYGWIN*) machine=Windows ;;
MINGW*) machine=Windows ;;
MSYS*) machine=Windows ;;
*) echo "Unknown system '${uname_output}'" && exit 1 ;;
esac
export MACHINE="$machine"

#if [[ "$machine" == "Windows" && ("$ARGS_CPP" == "" || "$ARGS_CPP" == "clang++") ]]; then
#  for f in msvc-${MSVC_INCLUDE_YEAR}-include windows-kit-${WINKIT_INCLUDE_VERSION}-include; do
#    rm -rf "${TESTS_DIR:?}"/lib/$f && mkdir -p "$TESTS_DIR"/lib/$f
#    7z x -o"${TESTS_DIR:?}"/lib/$f "$TESTS_DIR"/lib/$f.7z
#  done
#fi

run_tests() {
  export INVOCATION_ID=$RANDOM
  export CPP="$1"
  export CPP_STANDARD="$2"

  echo -e Running tests \("${CYAN}""$CPP"@"$CPP_STANDARD""${NC}"\)
  echo ""

  for test_dir in "$TESTS_DIR"/*/; do
    test=$(basename "$test_dir")
    if [[ $test == test-* ]]; then
      if [[ "$ARGS_TESTS" == "," || "$ARGS_TESTS" == *,$test,* ]]; then
        echo -e Running "${BLUE}${test}${NC}" \("$CPP"@"$CPP_STANDARD"\)
        export TEST_DIR="$test_dir"
        export CPP_INCLUDE_DIR=${TESTS_DIR}/..
        export CPP_OPTS=""
        if [[ -n "$ARGS_CPP_BITS" ]]; then
          export CPP_OPTS="-m$ARGS_CPP_BITS"
        fi
        pushd "$test_dir" 1>/dev/null 2>&1
        bash "${test_dir}run.sh"
        echo -e Done "${BLUE}$test${NC}" \("${CYAN}""$CPP"@"$CPP_STANDARD""${NC}"\)
        echo ""
        popd 1>&2 1>/dev/null 2>&1
      fi
    fi
  done

  rm -rf "$TESTS_DIR"/tester-lcmp
  echo -e Done all tests \("${CYAN}""$CPP"@"$CPP_STANDARD""${NC}"\)
  echo ""
}

VALGRIND=""
valgrind_output=$(valgrind 2>&1 || true)
if [[ $valgrind_output == *"--help"* ]]; then
  VALGRIND="valgrind -q"
  echo Valgrind mode is ON
else
  echo Valgrind mode is OFF
fi
export VALGRIND="$VALGRIND"

done=""
if [[ "$machine" == "Windows" && ("$ARGS_CPP" == "" || "$ARGS_CPP" == "msvc") ]]; then
  VS_RELEASES=("Professional" "Enterprise" "Community")
  PROGRAM_FILES=("${PROGRAMFILES}" "${PROGRAMFILES} (x86)")

  for vs_release in "${VS_RELEASES[@]}"; do
    for program_files in "${PROGRAM_FILES[@]}"; do
      for ((version = 2000; version <= 2100; version++)); do
        vs_dir="$program_files\\Microsoft Visual Studio\\$version\\$vs_release\\VC\\Auxiliary\\Build"
        if [[ -d $vs_dir ]]; then
          for bits in 32 64; do
            vcvars_bat_file="$vs_dir\\vcvars$bits.bat"
            if [[ -f $vcvars_bat_file ]]; then
              echo "Compiler Visual Studio $version ($vs_release-$bits) has been found"
              echo call \""$vcvars_bat_file"\" >do-vcvars.bat
              echo "bash -c export > vcvars.env" >>do-vcvars.bat
              python file-runner.py do-vcvars.bat
              grep -v -E "(\(.*=)|(\!.*=)|([A-Z]\-[A-Z].*=)" <vcvars.env >vcvars_filtered.env
              source vcvars_filtered.env
              rm -f do-vcvars.bat vcvars.env vcvars_filtered.env
              for cpp_standard in "${MSVC_CPP_STANDARDS[@]}"; do
                touch empty_file.cpp
                cpp_output=$(cl.exe "$cpp_standard" empty_file.cpp 2>&1 || true)
                rm -f empty_file.*
                if [[ ! $cpp_output == *"unknown"* ]]; then
                  echo Testing msvc-"$version"-$bits@"$cpp_standard"
                  run_tests "cl.exe" "$cpp_standard"
                  if [[ ! "$done" == "" ]]; then
                    done="$done, "
                  fi
                  done="${done}msvc-$version-$bits@$cpp_standard"
                fi
              done
            fi
          done
        fi
      done
    done
  done
fi

# Find /c/Programs/*/bin/g++ in case of Windows and no ARGS_CPP
if [[ "$MACHINE" == "Windows" && "$ARGS_CPP" == "" ]]; then
  for d in /c/Programs/*/; do
    dir="${d}bin"
    OLD_PATH="$PATH"
    export PATH="$dir":$PATH
    gpp="${d}bin/g++.exe"
    gpp_output=$($gpp 2>&1 || true)
    if [[ $gpp_output == *"no input files"* ]]; then
      for gpp_standard in "${CPP_STANDARDS[@]}"; do
        touch empty_file.cpp
        gpp_output=$($gpp "$gpp_standard" empty_file.cpp 2>&1 || true)
        if [[ ! $gpp_output == *"unrecognized"* && ! $gpp_output == *"standard"* ]]; then
          run_tests "$gpp" "$gpp_standard"
          if [[ ! "$done" == "" ]]; then
            done="$done, "
          fi
          done="$done$gpp@$gpp_standard"
        fi
        rm -f empty_file.*
      done
    fi
    export PATH="$OLD_PATH"
  done
fi

for compiler in "${COMPILERS[@]}"; do
  for version in 0 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20; do
    if [[ "$ARGS_CPP_VERSIONS" == "," || "$ARGS_CPP_VERSIONS" == *,$version,* ]]; then
      cpp="$compiler-$version"
      if [[ "$version" == 0 ]]; then
        cpp="$compiler"
      fi
      cpp_output=$($cpp 2>&1 || true)
      if [[ $cpp_output == *"no input files"* ]]; then
        echo "Compiler '$cpp' has been found"
        for cpp_standard in "${CPP_STANDARDS[@]}"; do
          touch empty_file.cpp
          cpp_output=$($cpp "$cpp_standard" empty_file.cpp 2>&1 || true)
          if [[ ! $cpp_output == *"unrecognized"* && ! $cpp_output == *"standard"* ]]; then
            if [[ "$machine" == "Windows" && "$cpp" == "clang++" && "$cpp_standard" == "--std=c++11" ]]; then
              echo Ignore "$cpp" "$cpp_standard" on $machine
              continue
            fi
            run_tests "$cpp" "$cpp_standard"
            if [[ ! "$done" == "" ]]; then
              done="$done, "
            fi
            done="$done$cpp@$cpp_standard"
          fi
          rm -f empty_file.*
        done
      fi
    fi
  done
done

#if [[ "$machine" == "Windows" ]]; then
#  for f in msvc-${MSVC_INCLUDE_YEAR}-include windows-kit-${WINKIT_INCLUDE_VERSION}-include; do
#    rm -rf "${TESTS_DIR:?}"/lib/$f
#  done
#fi

if [[ -z "$done" ]]; then
  echo -e "${RED}[ERROR]${NC} No compilers found\n"
  exit 1
fi

echo -e "${GREEN}Successfully tested on${NC}: $done\n"
