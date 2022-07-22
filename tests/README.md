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
