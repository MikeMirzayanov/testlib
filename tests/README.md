# Tests

Some features of the library are covered by tests. If you understand well how this or that functionality of the library works, then you can add a test for it.

First of all, consider adding a test to `test-004_use-test.h` (carefully study the design of this test).

Note the use of `test-ref` script in the tests, which checks that a process has exited with the expected exit code, standard output, and standard error.

It is highly desirable to cover all new functionality with tests. If the functionality is such that it is not very difficult to write tests, then the presence of tests is required.

## CI

On each push and pull requests tests will run. Please make sure that your changes didn't break them.

## How run tests locally

Use `tests\run.sh`. Also you can use `t.sh` (or `t.bat`) to run shortcut of `run.sh g++ 11 v0 $*`.

### Parameters (use can use all combinations):

* `g++`, `clang++`, `msvc` - to specify compiler(s)
* `11`, `14`, `17`, `20`, `23` - to specify standard(s)
* `vxx` (example `v10`) - to try `-10` suffix to run compilers, like `g++-10`, use `v0` to run without any suffixes
* `test-` - to specify test(s)

### Examples:
    
* `bash run.sh` - to run all tests on all compilers on multiple standards
* `bash run.sh g++` - to run all tests on `g++` on multiple standards
* `bash run.sh g++ 11` - to run all tests on `g++` with `--std=c++11`
* `bash run.sh g++ 11 test-001_run-sval` - to run `test-001_run-sval` on `g++` with `--std=c++11`
* `bash run.sh test-001_run-sval` - to run `test-001_run-sval` on all compilers on multiple standards
* `bash run.sh g++ 11 17` - to run all tests on `g++` with `--std=c++11` and with `--std=c++17`
