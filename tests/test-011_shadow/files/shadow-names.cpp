// The locals of the templates of testlib.h are named "_x": such names are reserved in the global
// namespace, so no program has a global of that name. None of them may be a macro either.
#include "testlib.h"

#if defined(_about) || defined(_ap) || defined(_buffer) || defined(_current) || defined(_d) \
    || defined(_digit) || defined(_expected) || defined(_expectedString) || defined(_first) \
    || defined(_foundString) || defined(_i) || defined(_it) || defined(_message) || defined(_n) \
    || defined(_negative) || defined(_p) || defined(_pos) || defined(_preparedVariableName) \
    || defined(_prepend) || defined(_repeated) || defined(_result) || defined(_result_sum) || defined(_s) \
    || defined(_septums) || defined(_sign) || defined(_size) || defined(_ss) || defined(_sum) \
    || defined(_t_string) || defined(_trailingZeroCount) || defined(_vals) || defined(_value) \
    || defined(_written) || defined(_x)
#error a name used for the locals of testlib templates is a macro
#endif

int main() {
    return 0;
}
