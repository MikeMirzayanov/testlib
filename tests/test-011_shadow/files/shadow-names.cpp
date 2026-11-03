// The parameters and locals of the templates of testlib.h are named "_x": such names are reserved in
// the global namespace, so no program has a global of that name. None of them may be a macro either.
#include "testlib.h"

#if defined(_a) || defined(_about) || defined(_ap) || defined(_args) || defined(_b) || defined(_begin) \
    || defined(_buffer) || defined(_c) || defined(_collection) || defined(_current) || defined(_d) \
    || defined(_default_value) || defined(_digit) || defined(_end) || defined(_expected) \
    || defined(_expectedString) || defined(_first) || defined(_fmt) || defined(_found) \
    || defined(_foundString) || defined(_from) || defined(_i) || defined(_index) || defined(_it) \
    || defined(_key) || defined(_last) || defined(_lower) || defined(_message) || defined(_min_part) \
    || defined(_n) || defined(_negative) || defined(_nonnegative) || defined(_p) || defined(_points) \
    || defined(_pos) || defined(_preparedVariableName) || defined(_prepend) || defined(_prependFormat) \
    || defined(_repeated) || defined(_rest) || defined(_result) || defined(_result_sum) || defined(_s) \
    || defined(_separator) || defined(_septums) || defined(_sign) || defined(_size) || defined(_ss) \
    || defined(_sum) || defined(_t) || defined(_t_string) || defined(_text) || defined(_to) \
    || defined(_total) || defined(_trailingZeroCount) || defined(_type) || defined(_upper) \
    || defined(_vals) || defined(_value) || defined(_variableName) || defined(_written) || defined(_x)
#error a name used for the parameters and locals of testlib templates is a macro
#endif

int main() {
    return 0;
}
